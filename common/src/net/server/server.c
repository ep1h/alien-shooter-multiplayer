#include "server.h"
#include <winsock2.h>
#include "../socket/socket.h"
#include "utils/mem/mem.h"
#include "utils/containers/queue/queue.h"
#include "utils/containers/pqueue/pqueue.h"
#include "utils/event/event.h"
#include "utils/thread/thread.h"
#include "utils/time/time.h"

typedef enum LoopStatus
{
    LS_STOPPED = 0,
    LS_WAITING_STOP = 1,
    LS_RUNNING = 2
} LoopStatus;

typedef struct Client
{
    bool is_connected;
    struct sockaddr_in address;
    NetTime last_recvfrom_time_ms;
} Client;

typedef struct NetServer
{
    NetSocket* socket;
    Queue* recv_queue;   /* PQueue of PacketInfo */
    PQueue* send_pqueue; /* PQueue of SendPacketInfo */
    Event* send_event;
    Thread* recv_thread;
    Thread* send_thread;
    Client* clients;
    LoopStatus recv_loop_status;
    LoopStatus send_loop_status;
    NetServerInfo info;
} NetServer;

typedef struct PacketInfo
{
    int size;
    NetPacket packet;
} PacketInfo;

typedef struct SendPacketInfo
{
    NetClientId destination;
    PacketInfo pi;
} SendPacketInfo;

static void recv_loop_(NetServer* server);
static void send_loop_(NetServer* server);
static NetClientId connect_client_(NetServer* server,
                                   const struct sockaddr_in* address);
static void disconnect_client_(NetServer* server, NetClientId id);
static void kick_delayed_clients_(NetServer* server);
static void clear_recv_queue_(NetServer* server);
static void clear_send_pqueue_(NetServer* server);
static void request_stop_recv_loop_(NetServer* server);
static void request_stop_send_loop_(NetServer* server);
static void wait_for_stop_recv_loop_(NetServer* server);
static void wait_for_stop_send_loop_(NetServer* server);

static void recv_loop_(NetServer* server)
{
    struct sockaddr_in sender;
    char buf[NET_SOCKET_MAX_MSG_SIZE];
    int size = 0;
    server->recv_loop_status = LS_RUNNING;
    while (server->recv_loop_status == LS_RUNNING)
    {
        size = net_socket_recvfrom(server->socket, &sender, buf);
        if (size >= (int)sizeof(NetPacket))
        {
            NetPacket* p = (NetPacket*)buf;
            switch (p->head.type)
            {
            case NPT_CONNECTION_REQUEST:
            {
                // TODO: Move this to another thread to prevent blocking recv
                //       loop by connection logic.

                /* Build NetPacketConnectionResponse*/
                NetPacketConnectionResponse pcresp;
                pcresp.head.type = NPT_CONNECTION_RESPONSE;
                pcresp.server_info.max_clients = server->info.max_clients;
                pcresp.assigned_id = connect_client_(server, &sender);
                pcresp.server_info.max_clients = server->info.max_clients;
                pcresp.server_info.recv_timeout_ms =
                    server->info.recv_timeout_ms;
                pcresp.server_info.send_timeout_ms =
                    server->info.send_timeout_ms;

                /* Send */
                net_socket_sendto(server->socket, &sender, &pcresp,
                                  sizeof(pcresp));

                break;
            }
            case NPT_SYNC:
            {
                if (net_server_is_client_connected(server, p->head.sender))
                {
                    server->clients[p->head.sender].last_recvfrom_time_ms =
                        time_get_ms();
                }
                break;
            }
            case NPT_DISCONNECT:
            {
                disconnect_client_(server, p->head.sender);
                break;
            }
            case NPT_DATA:
            {
                if (net_server_is_client_connected(server, p->head.sender))
                {
                    server->clients[p->head.sender].last_recvfrom_time_ms =
                        time_get_ms();
                    PacketInfo* pi = mem_alloc(sizeof(PacketInfo) + size);
                    pi->size = size;
                    mem_copy(&pi->packet, p, size);
                    queue_queue(server->recv_queue, pi);
                }
                break;
            }
            default:
            {
                break;
            }
            }
        }
        else if (size == NET_SOCKET_ERROR_TIMEDOUT)
        {
        }
        kick_delayed_clients_(server);
    }
    server->recv_loop_status = LS_STOPPED;
}

static void send_loop_(NetServer* server)
{
    server->send_loop_status = LS_RUNNING;
    while (server->send_loop_status == LS_RUNNING)
    {
        event_wait_timeout(server->send_event, server->info.send_timeout_ms);
        SendPacketInfo* spi;
        while ((spi = pqueue_dequeue(server->send_pqueue)))
        {
            if (spi->destination == NET_CLIENT_ID_ALL)
            {
                for (NetClientId i = 0; i < server->info.max_clients; i++)
                {
                    if (!server->clients[i].is_connected)
                        continue;
                    net_socket_sendto(server->socket,
                                      &server->clients[i].address,
                                      &spi->pi.packet, spi->pi.size);
                }
            }
            else if (spi->destination & NET_CLIENT_ID_ALL_BUT_FLAG)
            {
                for (NetClientId i = 0; i < server->info.max_clients; i++)
                {
                    if (!server->clients[i].is_connected ||
                        ((spi->destination & ~NET_CLIENT_ID_ALL_BUT_FLAG) == i))
                        continue;
                    net_socket_sendto(server->socket,
                                      &server->clients[i].address,
                                      &spi->pi.packet, spi->pi.size);
                }
            }
            else
            {
                /* Destination id upper limit has already checked */
                if (server->clients[spi->destination].is_connected)
                {
                    net_socket_sendto(
                        server->socket,
                        &server->clients[spi->destination].address,
                        &spi->pi.packet, spi->pi.size);
                }
            }
            mem_free(spi);
        }
    }
    server->send_loop_status = LS_STOPPED;
}

static NetClientId connect_client_(NetServer* server,
                                   const struct sockaddr_in* address)
{
    for (NetClientId i = 0; i < server->info.max_clients; i++)
    {
        Client* client = &server->clients[i];
        if (!client->is_connected)
        {
            mem_copy(&client->address, address, sizeof(struct sockaddr_in));
            client->last_recvfrom_time_ms = time_get_ms();
            client->is_connected = true;

            PacketInfo* pi =
                mem_alloc(sizeof(PacketInfo) + sizeof(NetPacketVirtual) -
                          sizeof(NetPacketHead));
            pi->packet.head.type = NPT_VIRTUAL;
            pi->packet.head.sender = i;
            pi->size = sizeof(NetPacketVirtual);
            ((NetPacketVirtual*)(&pi->packet))->virtual_type =
                NVPT_CLIENT_CONNECTED;

            queue_queue(server->recv_queue, pi);
            return i;
        }
    }
    return NET_CLIENT_ID_INVALID;
}

static void disconnect_client_(NetServer* server, NetClientId id)
{
    if (id < server->info.max_clients && server->clients[id].is_connected)
    {
        mem_set(&server->clients[id], 0, sizeof(Client));

        PacketInfo* pi =
            mem_alloc(sizeof(PacketInfo) + sizeof(NetPacketVirtual) -
                      sizeof(NetPacketHead));
        pi->packet.head.type = NPT_VIRTUAL;
        pi->packet.head.sender = id;
        pi->size = sizeof(NetPacketVirtual);
        ((NetPacketVirtual*)(&pi->packet))->virtual_type =
            NVPT_CLIENT_DISCONNECTED;
        queue_queue(server->recv_queue, pi);
    }
}

static void kick_delayed_clients_(NetServer* server)
{
    static NetTime _last_check_time_ms = 0;
    NetTime time_ms = time_get_ms();
    if (time_ms - _last_check_time_ms >= server->info.recv_timeout_ms)
    {
        for (NetClientId i = 0; i < server->info.max_clients; i++)
        {
            if (server->clients->is_connected)
            {
                if (time_ms - server->clients[i].last_recvfrom_time_ms >=
                    server->info.recv_timeout_ms)
                {
                    disconnect_client_(server, i);
                }
            }
        }
        _last_check_time_ms = time_ms;
    }
}

static void clear_recv_queue_(NetServer* server)
{
    void* recv_queue_item = 0;
    while ((recv_queue_item = queue_dequeue(server->recv_queue)))
    {
        mem_free(recv_queue_item);
    }
}

static void clear_send_pqueue_(NetServer* server)
{
    void* send_pqueue_item = 0;
    while ((send_pqueue_item = pqueue_dequeue(server->send_pqueue)))
    {
        mem_free(send_pqueue_item);
    }
}

static void request_stop_recv_loop_(NetServer* server)
{
    server->recv_loop_status = LS_WAITING_STOP;
}

static void request_stop_send_loop_(NetServer* server)
{
    server->send_loop_status = LS_WAITING_STOP;
}

static void wait_for_stop_recv_loop_(NetServer* server)
{
    while (server->recv_loop_status != LS_STOPPED)
    {
        time_sleep_ms(1);
    }
}

static void wait_for_stop_send_loop_(NetServer* server)
{
    while (server->send_loop_status != LS_STOPPED)
    {
        time_sleep_ms(1);
    }
}

NetServer* net_server_create(unsigned short port, NetClientId max_clients,
                             NetTime recv_timeout_ms, NetTime send_timeout_ms)
{
    if (!net_socket_init())
    {
        return 0;
    }
    NetServer* server = mem_alloc(sizeof(NetServer));
    if (server)
    {
        mem_set(server, 0, sizeof(NetServer));
        server->info.max_clients = max_clients;
        server->info.recv_timeout_ms = recv_timeout_ms;
        server->info.send_timeout_ms = send_timeout_ms;
        server->clients = mem_alloc(max_clients * sizeof(Client));
        if (server->clients)
        {
            mem_set(server->clients, 0, max_clients * sizeof(Client));

            server->socket = net_socket_create_socket();
            if (server->socket)
            {
                net_socket_set_recvfrom_timeout(server->socket,
                                                server->info.recv_timeout_ms);
                net_socket_set_sendto_timeout(server->socket,
                                              server->info.send_timeout_ms);
                if (net_socket_bind_socket(server->socket, 0, port))
                {
                    server->recv_queue = queue_create_fifo();
                    if (server->recv_queue)
                    {
                        server->send_pqueue = pqueue_create_fifo();
                        if (server->send_pqueue)
                        {
                            server->send_event = event_create();
                            if (server->send_event)
                            {
                                server->recv_thread =
                                    thread_create_with_args(recv_loop_, server);
                                if (server->recv_thread)
                                {
                                    server->send_thread =
                                        thread_create_with_args(send_loop_,
                                                                server);
                                    if (server->send_thread)
                                    {
                                        mem_set(server->clients, 0,
                                                sizeof(server->clients));
                                        return server;
                                    }
                                    thread_destroy(server->send_thread, 0);
                                    server->send_thread = 0;
                                }
                                event_destroy(server->send_event);
                                server->send_event = 0;
                            }
                            pqueue_dequeue(server->send_pqueue);
                            server->send_pqueue = 0;
                        }
                        queue_destroy(server->recv_queue);
                        server->recv_queue = 0;
                    }
                }
                net_socket_destroy_socket(server->socket);
            }
            mem_free(server->clients);
        }
        mem_free(server);
    }
    return 0;
}

void net_server_destroy(NetServer* server)
{
    request_stop_recv_loop_(server);
    request_stop_send_loop_(server);
    if (server->send_thread)
    {
        wait_for_stop_send_loop_(server);
        thread_destroy(server->send_thread, 0);
        server->send_thread = 0;
    }
    if (server->recv_thread)
    {
        wait_for_stop_recv_loop_(server);
        thread_destroy(server->recv_thread, 0);
        server->recv_thread = 0;
    }
    if (server->send_event)
    {
        event_destroy(server->send_event);
        server->send_event = 0;
    }
    if (server->send_pqueue)
    {
        clear_send_pqueue_(server);
        pqueue_destroy(server->send_pqueue);
        server->send_pqueue = 0;
    }
    if (server->recv_queue)
    {
        clear_recv_queue_(server);
        queue_destroy(server->recv_queue);
        server->recv_queue = 0;
    }
    if (server->socket)
    {
        net_socket_destroy_socket(server->socket);
        server->socket = 0;
    }
    if (server->clients)
    {
        mem_free(server->clients);
        server->clients = 0;
    }
    if (server)
    {
        mem_free(server);
        server = 0;
    }
}

int net_server_dequeue_packet(NetServer* server, NetPacket* packet)
{
    PacketInfo* pi = queue_dequeue(server->recv_queue);
    if (pi)
    {
        mem_copy(packet, &pi->packet, pi->size);
        int result = pi->size;
        mem_free(pi);
        return result;
    }
    return 0;
}

void net_server_send(NetServer* server, NetClientId id, NetPacket* packet,
                     int size, NetPriority priority)
{
    if ((id & ~NET_CLIENT_ID_ALL_BUT_FLAG) >= server->info.max_clients)
    {
        return;
    }
    SendPacketInfo* spi =
        mem_alloc(sizeof(SendPacketInfo) + size - sizeof(NetPacket));
    mem_copy(&spi->pi.packet, packet, size);
    spi->pi.size = size;
    spi->destination = id;
    pqueue_queue(server->send_pqueue, spi, priority);
    event_trigger(server->send_event);
}

bool net_server_is_client_connected(NetServer* server, NetClientId id)
{
    return server && id < server->info.max_clients &&
           server->clients[id].is_connected;
}

const NetServerInfo* net_server_get_info(NetServer* server)
{
    if (server)
    {
        return &server->info;
    }
    return 0;
}
