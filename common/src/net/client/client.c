#include <winsock2.h>
#include "client.h"
#include "../socket/socket.h"
#include "utils/mem/mem.h"
#include "utils/containers/queue/queue.h"
#include "utils/containers/pqueue/pqueue.h"
#include "utils/time/time.h"

#define NET_CLIENT_DEFAULT_RECV_TIMEOUT_MS 5000
#define NET_CLIENT_DEFAULT_SEND_TIMEOUT_MS 5000

typedef struct PacketInfo
{
    int size;
    NetPacket packet;
} PacketInfo;

typedef struct NetClient
{
    NetClientState state;
    NetSocket* socket;
    struct sockaddr_in server_address;
    Queue* recv_queue;   /* Queue of PacketInfo */
    PQueue* send_pqueue; /* PQueue of SendPacketInfo */
    NetClientId id;
    NetServerInfo server_info;
    NetTime sync_period_ms; // TODO: Remove ?
    NetTime last_send_time_ms;
    NetTime connection_request_time_ms;
} NetClient;

static void state_connecting_handler_(NetClient* client);
static void state_connection_failed_handler_(NetClient* client);
static void state_connected_handler_(NetClient* client);
static void state_disconnection_handler_(NetClient* client);

static void state_connecting_handler_(NetClient* client)
{
    NetTime cur_time = time_get_ms();
    if ((cur_time - client->connection_request_time_ms) >
        NET_CLIENT_DEFAULT_RECV_TIMEOUT_MS)
    {
        /* Connection failed (invalid server address/connection request lost) */
        // TODO: Do 1 more attempt.
        client->state = NCS_CONNECTION_FAILED;
        return;
    }

    /* Wait for connection response */
    struct sockaddr_in addr;
    unsigned char
        buf[NET_SOCKET_MAX_MSG_SIZE]; // TODO: Use
                                      // sizeof(NetPacketConnectionResponse)
    NetPacketConnectionResponse* pcresp = (NetPacketConnectionResponse*)buf;
    int size = net_socket_recvfrom(client->socket, &addr, buf);
    // TODO: Check sender address.
    if (size > 0 && pcresp->head.type == NPT_CONNECTION_RESPONSE)
    {
        if (pcresp->assigned_id != NET_CLIENT_ID_INVALID)
        {
            client->id = pcresp->assigned_id;
            client->server_info.max_clients = pcresp->server_info.max_clients;
            client->server_info.recv_timeout_ms =
                pcresp->server_info.recv_timeout_ms;
            client->server_info.send_timeout_ms =
                pcresp->server_info.send_timeout_ms;
            /* Calculate period for sending sync packets */
            client->sync_period_ms = client->server_info.recv_timeout_ms * 0.7f;
            /* Reconfigure socket */
            net_socket_set_recvfrom_timeout(
                client->socket, client->server_info.recv_timeout_ms);
            net_socket_set_sendto_timeout(client->socket,
                                          client->server_info.send_timeout_ms);
            client->last_send_time_ms = time_get_ms();
            client->state = NCS_CONNECTED;
        }
        else
        {
            /* Server is full */
            client->state = NCS_CONNECTION_FAILED;
        }
    }
}

static void state_connection_failed_handler_(NetClient* client)
{
    (void)client;
}

static void state_connected_handler_(NetClient* client)
{
    /* Process data to be send */
    NetTime cur_time = time_get_ms();
    PacketInfo* pi;
    while ((pi = pqueue_dequeue(client->send_pqueue)))
    {
        net_socket_sendto(client->socket, &client->server_address, &pi->packet,
                          pi->size);
        client->last_send_time_ms = cur_time;
        mem_free(pi);
    }
    /* Send SYNC if required */
    if ((cur_time - client->last_send_time_ms) >= client->sync_period_ms)
    {
        NetPacketSync nps;
        nps.head.type = NPT_SYNC;
        nps.head.sender = client->id;
        net_socket_sendto(client->socket, &client->server_address, &nps,
                          sizeof(nps));
        client->last_send_time_ms = cur_time;
    }

    /* Process receiced data */
    struct sockaddr_in sender;
    char buf[NET_SOCKET_MAX_MSG_SIZE];
    int size;
    while ((size = net_socket_recvfrom(client->socket, &sender, buf)) > 0)
    {
        // TODO: Filter out packets not from server
        //  if (((NetPacket *)buf)->head.sender == client->id)
        //  {
        //      continue;
        //  }
        PacketInfo* pi = mem_alloc(sizeof(PacketInfo) + size);
        pi->size = size;
        mem_copy(&pi->packet, buf, size);
        queue_queue(client->recv_queue, pi);
    }
}

static void state_disconnection_handler_(NetClient* client)
{
    /* Clear recv queue */
    void* item_ptr = 0;
    while ((item_ptr = queue_dequeue(client->recv_queue)))
    {
        mem_free(item_ptr);
    }
    /* Clear send queue */
    while ((item_ptr = pqueue_dequeue(client->send_pqueue)))
    {
        mem_free(item_ptr);
    }
    /* Clear server info */
    mem_set(&client->server_info, 0, sizeof(client->server_info));
    /* Clear server address */
    mem_set(&client->server_address, 0, sizeof(client->server_address));
    /* Reset client id */
    client->id = NET_CLIENT_ID_INVALID;
    /* Switch state to disconnected */
    client->state = NCS_DISCONNECTED;
}

NetClient* net_client_create(void)
{
    if (net_socket_init()) // TODO: Add check.
    {
        NetClient* client = mem_alloc(sizeof(NetClient));
        if (client)
        {
            mem_set(client, 0, sizeof(NetClient));
            client->id = NET_CLIENT_ID_INVALID;
            client->socket = net_socket_create_socket();
            if (client->socket)
            {
                net_socket_set_nonblocking(client->socket, true);
                net_socket_set_recvfrom_timeout(
                    client->socket, NET_CLIENT_DEFAULT_RECV_TIMEOUT_MS);
                net_socket_set_sendto_timeout(
                    client->socket, NET_CLIENT_DEFAULT_SEND_TIMEOUT_MS);
                client->recv_queue = queue_create_fifo();
                if (client->recv_queue)
                {
                    client->send_pqueue = pqueue_create_fifo();
                    if (client->send_pqueue)
                    {
                        client->state = NCS_INITED;
                        return client;
                        // pqueue_destroy(client->send_pqueue);
                    }
                    queue_destroy(client->recv_queue);
                }
                net_socket_destroy_socket(client->socket);
            }
            mem_free(client);
        }
        net_socket_destroy();
    }
    return 0;
}

void net_client_destroy(NetClient* client)
{
    if (client)
    {
        if (client->socket)
        {
            if (client->recv_queue)
            {
                if (client->send_pqueue)
                {
                    pqueue_destroy(client->send_pqueue);
                }
                queue_destroy(client->recv_queue);
            }
            net_socket_destroy_socket(client->socket);
        }
        mem_free(client);
    }
    net_socket_destroy();
}

bool net_client_connection_request(NetClient* client, const char* ip,
                                   NetPort port)
{
    if (client && ((client->state == NCS_INITED) ||
                   (client->state == NCS_CONNECTION_FAILED) ||
                   (client->state == NCS_DISCONNECTED)))
    {
        client->server_address.sin_family = AF_INET;
        client->server_address.sin_port = htons(port);
        client->server_address.sin_addr.S_un.S_addr = inet_addr(ip);

        /* Build NetPacketConnectionRequest */
        NetPacketConnectionRequest pcreq;
        pcreq.head.sender = NET_CLIENT_ID_INVALID;
        pcreq.head.type = NPT_CONNECTION_REQUEST;
        /* Send NetPacketConnectionRequest */
        if (net_socket_sendto(client->socket, &client->server_address, &pcreq,
                              sizeof(pcreq)) == sizeof(pcreq))
        {
            client->connection_request_time_ms = time_get_ms();
            client->state = NCS_CONNECTING;
            return true;
        }
    }
    return false;
}

void net_client_disconnect(NetClient* client)
{
    if (client &&
        (client->state == NCS_CONNECTED || client->state == NCS_CONNECTING))
    {
        /* Build NetPacketDisconnect */
        NetPacketDisconnect pd;
        pd.head.sender = client->id;
        pd.head.type = NPT_DISCONNECT;
        /* Send NetPacketConnectionRequest */
        net_socket_sendto(client->socket, &client->server_address, &pd,
                          sizeof(pd));
        client->state = NCS_DISCONNECTION;
    }
}

void net_client_tick(NetClient* client)
{
    if (!client)
    {
        return;
    }
    switch (client->state)
    {
    case NCS_UNINITED:
    {

        break;
    }
    case NCS_INITED:
    {

        break;
    }
    case NCS_CONNECTING:
    {
        state_connecting_handler_(client);
        break;
    }
    case NCS_CONNECTION_FAILED:
    {
        state_connection_failed_handler_(client);
        break;
    }
    case NCS_CONNECTED:
    {
        state_connected_handler_(client);
        break;
    }
    case NCS_DISCONNECTION:
    {
        state_disconnection_handler_(client);
        break;
    }
    case NCS_DISCONNECTED:
    {
    }
    }
}

int net_client_dequeue_packet(NetClient* client, NetPacket* packet)
{
    if (client && client->recv_queue)
    {
        PacketInfo* pi = queue_dequeue(client->recv_queue);
        if (pi)
        {
            mem_copy(packet, &pi->packet, pi->size);
            int result = pi->size;
            mem_free(pi);
            return result;
        }
    }
    return 0;
}

void net_client_send(NetClient* client, const void* buf, int size,
                     NetPriority priority)
{
    if (client && client->state == NCS_CONNECTED && client->send_pqueue)
    {
        PacketInfo* pi = mem_alloc(sizeof(PacketInfo) + size);
        pi->packet.head.sender = client->id;
        pi->packet.head.type = NPT_DATA;
        mem_copy(&pi->packet.payload, buf, size);
        pi->size = size + sizeof(NetPacket);
        pqueue_queue(client->send_pqueue, pi, priority);
    }
}

NetClientState net_client_get_state(NetClient* client)
{
    if (client)
    {
        return client->state;
    }
    return 0;
}

const NetServerInfo* net_client_get_server_info(NetClient* client)
{
    if (client)
    {
        return &client->server_info;
    }
    return 0;
}

NetClientId net_client_get_id(NetClient* client)
{
    if (client)
    {
        return client->id;
    }
    return NET_CLIENT_ID_INVALID;
}
