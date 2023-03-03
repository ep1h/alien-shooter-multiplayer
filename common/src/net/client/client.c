#include "client.h"
#include <winsock2.h>
#include "../socket/socket.h"
#include "utils/mem/mem.h"
#include "utils/containers/queue/queue.h"
#include "utils/containers/pqueue/pqueue.h"
#include "utils/event/event.h"
#include "utils/thread/thread.h"
#include "utils/time/time.h"

#define NET_CLIENT_DEFAULT_RECV_TIMEOUT_MS 6000
#define NET_CLIENT_DEFAULT_SEND_TIMEOUT_MS 6000

typedef enum LoopStatus
{
    LS_STOPPED = 0,
    LS_WAITING_STOP = 1,
    LS_RUNNING = 2
} LoopStatus;

typedef struct NetClient
{
    NetSocket* socket;
    struct sockaddr_in server_address;
    NetServerInfo server_info;
    NetTime sync_period_ms;
    Queue* recv_queue;   /* PQueue of PacketInfo */
    PQueue* send_pqueue; /* PQueue of SendPacketInfo */
    Event* send_event;
    Thread* recv_thread;
    Thread* send_thread;
    NetClientId id;
    LoopStatus recv_loop_status;
    LoopStatus send_loop_status;
    bool is_connected;
} NetClient;

typedef struct PacketInfo
{
    int size;
    NetPacket packet;
} PacketInfo;

static void recv_loop_(NetClient* client);
static void send_loop_(NetClient* client);
static void clear_recv_queue_(NetClient* client);
static void clear_send_pqueue_(NetClient* client);
static void request_stop_recv_loop_(NetClient* server);
static void request_stop_send_loop_(NetClient* server);
static void wait_for_stop_recv_loop_(NetClient* server);
static void wait_for_stop_send_loop_(NetClient* server);

static void recv_loop_(NetClient* client)
{
    struct sockaddr_in sender;
    char buf[NET_SOCKET_MAX_MSG_SIZE];
    int size = 0;
    client->recv_loop_status = LS_RUNNING;
    while (client->is_connected)
    {
        size = net_socket_recvfrom(client->socket, &sender, buf);
        // TODO: Filter out packets not from server
        if (size > 0)
        {
            // printf("- - - - - recv: type %d from %d\n",
            // ((NetPacketHead*)buf)->type, ((NetPacketHead*)buf)->sender);
            //  if (((NetPacket *)buf)->head.sender == client->id)
            //  {
            //      continue;
            //  }
            PacketInfo* pi = mem_alloc(sizeof(PacketInfo) + size);
            pi->size = size;
            mem_copy(&pi->packet, buf, size);
            queue_queue(client->recv_queue, pi);
            // printf("*** CLIENT *** RECV %d bytes from %d\n", size,
            //        pi->packet.head.sender);
        }
        else if (size == NET_SOCKET_ERROR_TIMEDOUT)
        {
            // printf("c: recv timeout\n");
        }
    }
    client->recv_loop_status = LS_STOPPED;
    thread_exit_current(0);
}

static void send_loop_(NetClient* client)
{
    client->send_loop_status = LS_RUNNING;
    while (client->is_connected)
    {
        if (event_wait_timeout(client->send_event, client->sync_period_ms))
        {
            PacketInfo* pi;
            while ((pi = pqueue_dequeue(client->send_pqueue)))
            {
                net_socket_sendto(client->socket, &client->server_address,
                                  &pi->packet, pi->size);
                mem_free(pi);
            }
        }
        else
        {
            NetPacketSync nps;
            nps.head.type = NPT_SYNC;
            nps.head.sender = client->id;
            net_socket_sendto(client->socket, &client->server_address, &nps,
                              sizeof(nps));
        }
    }
    client->send_loop_status = LS_STOPPED;
    thread_exit_current(0);
}

static void clear_recv_queue_(NetClient* client)
{
    void* recv_queue_item = 0;
    while ((recv_queue_item = queue_dequeue(client->recv_queue)))
    {
        mem_free(recv_queue_item);
    }
}

static void clear_send_pqueue_(NetClient* client)
{
    void* send_pqueue_item = 0;
    while ((send_pqueue_item = pqueue_dequeue(client->send_pqueue)))
    {
        mem_free(send_pqueue_item);
    }
}

static void request_stop_recv_loop_(NetClient* client)
{
    client->recv_loop_status = LS_WAITING_STOP;
}

static void request_stop_send_loop_(NetClient* client)
{
    client->send_loop_status = LS_WAITING_STOP;
}

static void wait_for_stop_recv_loop_(NetClient* client)
{
    while (client->recv_loop_status != LS_STOPPED)
    {
        time_sleep_ms(1);
    }
}

static void wait_for_stop_send_loop_(NetClient* client)
{
    while (client->send_loop_status != LS_STOPPED)
    {
        time_sleep_ms(1);
    }
}

NetClient* net_client_create(void)
{
    /* Init sockets */
    net_socket_init();
    /* Create Client object */
    NetClient* client = mem_alloc(sizeof(NetClient));
    mem_set(client, 0, sizeof(NetClient));
    client->id = NET_CLIENT_ID_INVALID;
    client->is_connected = false;

    /* Create socket */
    client->socket = net_socket_create_socket();
    /* Configure socket with default settings */
    net_socket_set_recvfrom_timeout(client->socket,
                                    NET_CLIENT_DEFAULT_RECV_TIMEOUT_MS);
    net_socket_set_sendto_timeout(client->socket,
                                  NET_CLIENT_DEFAULT_SEND_TIMEOUT_MS);
    // TODO: configure max recv. msg len for the socket.
    return client;
}

void net_client_destroy(NetClient* client)
{
    net_socket_destroy_socket(client->socket);
    mem_free(client);
}

bool net_client_connect(NetClient* client, const char* ip, unsigned short port)
{
    /* Close previous connection (if exists) */
    if (client->is_connected)
    {
        net_client_disconnect(client);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.S_un.S_addr = inet_addr(ip);
    NetClientId client_id = NET_CLIENT_ID_INVALID;

    /* Build NetPacketConnectionRequest */
    NetPacketConnectionRequest pcreq;
    pcreq.head.sender = client->id;
    pcreq.head.type = NPT_CONNECTION_REQUEST;
    unsigned char buf[NET_SOCKET_MAX_MSG_SIZE] = {0};
    /* Send NetPacketConnectionRequest */
    int size = net_socket_sendto(client->socket, &server_address, &pcreq,
                                 sizeof(pcreq));
    if (size != sizeof(pcreq))
    {
        return false;
    }
    /* Wait for PacketConnectionResponse */
    /* If multiple clients connect from the same address, packets other than the
     * PacketConnectionResponse may arrive at the current time. The following
     * loop is to skip them. */
    NetPacketConnectionResponse* pcresp = (NetPacketConnectionResponse*)buf;
    for (int i = 0; i < 5; i++)
    {
        struct sockaddr_in addr;
        size = net_socket_recvfrom(client->socket, &addr, buf);
        // TODO: Check sender address.
        if (size != sizeof(NetPacketConnectionResponse) ||
            pcresp->head.type != NPT_CONNECTION_RESPONSE)
        {
            continue;
        }
        if (pcresp->assigned_id == NET_CLIENT_ID_INVALID)
        {
            /* Server is full */
            return false;
        }
        else
        {
            /* Success. Store assigned client id */
            client_id = pcresp->assigned_id;
            /* Store server info */
            client->server_info.max_clients = pcresp->server_info.max_clients;
            client->server_info.recv_timeout_ms =
                pcresp->server_info.recv_timeout_ms;
            client->server_info.send_timeout_ms =
                pcresp->server_info.send_timeout_ms;
            /* Calculate period for sending sync packet */
            client->sync_period_ms = client->server_info.recv_timeout_ms * 0.8;
            /* Reconfigure socket */
            net_socket_set_recvfrom_timeout(
                client->socket, client->server_info.recv_timeout_ms);
            net_socket_set_sendto_timeout(client->socket,
                                          client->server_info.send_timeout_ms);
            break;
        }
    }
    if (client_id == NET_CLIENT_ID_INVALID)
    {
        return false;
    }

    /* Set server address */
    mem_copy(&client->server_address, &server_address, sizeof(server_address));

    /* Set client id */
    client->id = client_id;

    /* Set connection status */
    client->is_connected = true;

    /* Create recv queue */
    client->recv_queue = queue_create_fifo();
    if (client->recv_queue)
    {
        /* Create send pqueue */
        client->send_pqueue = pqueue_create_fifo();
        if (client->send_pqueue)
        {
            /* Create send event */
            client->send_event = event_create();
            if (client->send_event)
            {
                /* Start recv loop */
                client->recv_thread =
                    thread_create_with_args(recv_loop_, client);
                if (client->recv_thread)
                {
                    /* Start send loop */
                    client->send_thread =
                        thread_create_with_args(send_loop_, client);
                    if (client->send_thread)
                    {
                        return true;
                    }
                    thread_destroy(client->recv_thread, 0);
                    client->recv_thread = 0;
                }
                event_destroy(client->send_event);
                client->send_event = 0;
            }
            pqueue_destroy(client->send_pqueue);
            client->send_pqueue = 0;
        }
        queue_destroy(client->recv_queue);
        client->recv_queue = 0;
    }
    /* Reset client id */
    client->id = NET_CLIENT_ID_INVALID;
    /* Reset connection status */
    client->is_connected = false;
    return false;
}

void net_client_disconnect(NetClient* client)
{
    if (!client)
    {
        return;
    }

    /* Build NetPacketDisconnect */
    NetPacketDisconnect pd;
    pd.head.sender = client->id;
    pd.head.type = NPT_DISCONNECT;
    /* Send NetPacketConnectionRequest */
    net_socket_sendto(client->socket, &client->server_address, &pd, sizeof(pd));

    /* Change connection status */
    client->is_connected = false;

    /* Reset client id */
    client->id = NET_CLIENT_ID_INVALID;

    request_stop_send_loop_(client);
    request_stop_recv_loop_(client);
    /* Destroy send thread */
    if (client->send_thread)
    {
        wait_for_stop_send_loop_(client);

        thread_destroy(client->send_thread, 0);
        client->send_thread = 0;
    }
    /* Destroy recv thread */
    if (client->recv_thread)
    {
        wait_for_stop_recv_loop_(client);
        thread_destroy(client->recv_thread, 0);
        client->recv_thread = 0;
    }
    /* Destroy send event */
    if (client->send_event)
    {
        event_destroy(client->send_event);
        client->send_event = 0;
    }
    /* Destroy send pqueue */
    if (client->send_pqueue)
    {
        clear_send_pqueue_(client);
        pqueue_destroy(client->send_pqueue);
        client->send_pqueue = 0;
    }
    /* Destroy recv queue */
    if (client->recv_queue)
    {
        clear_recv_queue_(client);
        queue_destroy(client->recv_queue);
        client->recv_queue = 0;
    }
}

int net_client_dequeue_packet(NetClient* client, NetPacket* packet)
{
    PacketInfo* pi = queue_dequeue(client->recv_queue);
    if (pi)
    {
        mem_copy(packet, &pi->packet, pi->size);
        int result = pi->size;
        mem_free(pi);
        return result;
    }
    return 0;
}

void net_client_send(NetClient* client, const void* buf, int size,
                     NetPriority priority)
{
    if (!client || !client->is_connected)
    {
        // printf("net_client_send error!\n");
        return;
    }
    PacketInfo* pi = mem_alloc(sizeof(PacketInfo) + size);
    pi->packet.head.sender = client->id;
    pi->packet.head.type = NPT_DATA;
    mem_copy(&pi->packet.payload, buf, size);
    pi->size = size + sizeof(NetPacket);
    pqueue_queue(client->send_pqueue, pi, priority);
    event_trigger(client->send_event);
}

bool net_client_is_connected(NetClient* client)
{
    return client->is_connected;
}

NetClientId net_client_get_id(NetClient* client)
{
    return client->id;
}
