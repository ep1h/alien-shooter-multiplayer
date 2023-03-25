#include "server.h"
#include <winsock2.h>
#include "../socket/socket.h"
#include "utils/mem/mem.h"
#include "utils/containers/queue/queue.h"
#include "utils/containers/pqueue/pqueue.h"
#include "utils/time/time.h"
#include <stdio.h>

typedef struct Client
{
    bool is_connected;
    struct sockaddr_in address;
    NetTime last_recvfrom_time_ms;
} Client;

typedef struct NetServer
{
    NetSocket* socket;
    Queue* recv_queue;   /* Queue of CPacketInfo*/
    PQueue* send_pqueue; /* PQueue of SPacketInfo */
    Client* clients;
    NetServerInfo info;
    NetTime time_ms;
    NetTime last_check_delayed_clients_ms;
} NetServer;

typedef struct CPacketInfo
{
    int size;
    NetCPacket packet;
} CPacketInfo;

typedef struct SPacketInfo
{
    int size;
    NetSPacket packet;
} SPacketInfo;

typedef struct SendPacketInfo
{
    NetClientId destination;
    SPacketInfo pi;
} SendPacketInfo;

static void recv_(NetServer* server);
static void send_(NetServer* server);
static NetClientId connect_client_(NetServer* server,
                                   const struct sockaddr_in* address);
static void disconnect_client_(NetServer* server, NetClientId id);
static void kick_delayed_clients_(NetServer* server);
static void clear_recv_queue_(NetServer* server);
static void clear_send_pqueue_(NetServer* server);

static void recv_(NetServer* server)
{
    struct sockaddr_in sender;
    char buf[NET_SOCKET_MAX_MSG_SIZE];
    int size = 0;
    while ((size = net_socket_recvfrom(server->socket, &sender, buf)) > 0)
    {
        if (size < (int)sizeof(NetCPacket))
        {
            continue;
        }
        NetCPacket* p = (NetCPacket*)buf;
        switch (p->chead.net_head.type)
        {
        case NPT_C_CONNECTION_REQUEST:
        {
            /* Build NetSPacketConnectionResponse*/
            NetSPacketConnectionResponse pcresp;
            pcresp.head.net_head.type = NPT_S_CONNECTION_RESPONSE;
            pcresp.server_info.max_clients = server->info.max_clients;
            pcresp.assigned_id = connect_client_(server, &sender);
            pcresp.server_info.max_clients = server->info.max_clients;
            pcresp.server_info.recv_timeout_ms = server->info.recv_timeout_ms;
            pcresp.server_info.send_timeout_ms = server->info.send_timeout_ms;

            /* Send connection response */
            net_socket_sendto(server->socket, &sender, &pcresp, sizeof(pcresp));

            break;
        }
        case NPT_C_SYNC:
        {
            if (net_server_is_client_connected(server, p->chead.sender))
            {
                server->clients[p->chead.sender].last_recvfrom_time_ms =
                    server->time_ms;
            }
            break;
        }
        case NPT_C_DISCONNECT:
        {
            disconnect_client_(server, p->chead.sender);
            break;
        }
        case NPT_C_DATA:
        {
            if (net_server_is_client_connected(server, p->chead.sender))
            {
                server->clients[p->chead.sender].last_recvfrom_time_ms =
                    server->time_ms;
                CPacketInfo* pi = mem_alloc(sizeof(CPacketInfo) + size);
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
}

static void send_(NetServer* server)
{
    SendPacketInfo* spi;
    while ((spi = pqueue_dequeue(server->send_pqueue)))
    {
        if (spi->destination == NET_CLIENT_ID_ALL)
        {
            for (NetClientId i = 0; i < server->info.max_clients; i++)
            {
                if (!server->clients[i].is_connected)
                    continue;
                net_socket_sendto(server->socket, &server->clients[i].address,
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
                net_socket_sendto(server->socket, &server->clients[i].address,
                                  &spi->pi.packet, spi->pi.size);
            }
        }
        else
        {
            /* Destination id upper limit has already checked */
            if (server->clients[spi->destination].is_connected)
            {
                net_socket_sendto(server->socket,
                                  &server->clients[spi->destination].address,
                                  &spi->pi.packet, spi->pi.size);
            }
        }
        mem_free(spi);
    }
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
            client->last_recvfrom_time_ms = server->time_ms;
            client->is_connected = true;
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
    }
}

static void kick_delayed_clients_(NetServer* server)
{
    for (NetClientId i = 0; i < server->info.max_clients; i++)
    {
        if (server->clients[i].is_connected)
        {
            if ((server->time_ms - server->clients[i].last_recvfrom_time_ms) >=
                server->info.recv_timeout_ms)
            {
                printf("kick %d (%ld | %ld)", i, server->time_ms,
                       server->clients[i].last_recvfrom_time_ms);
                disconnect_client_(server, i);
            }
        }
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
                net_socket_set_nonblocking(server->socket, true);
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
                            mem_set(server->clients, 0,
                                    sizeof(server->clients));
                            return server;
                        }
                        pqueue_destroy(server->send_pqueue);
                        server->send_pqueue = 0;
                    }
                    queue_destroy(server->recv_queue);
                    server->recv_queue = 0;
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
    if (server->recv_queue)
    {
        clear_recv_queue_(server);
        queue_destroy(server->recv_queue);
        server->recv_queue = 0;
    }
    if (server->send_pqueue)
    {
        clear_send_pqueue_(server);
        pqueue_destroy(server->send_pqueue);
        server->send_pqueue = 0;
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

void net_server_tick(NetServer* server)
{
    if (server)
    {
        server->time_ms = time_get_ms();
        recv_(server);
        send_(server);
        kick_delayed_clients_(server);
    }
}

int net_server_dequeue_packet(NetServer* server, NetCPacket* packet)
{
    CPacketInfo* pi = queue_dequeue(server->recv_queue);
    if (pi)
    {
        mem_copy(packet, &pi->packet, pi->size);
        int result = pi->size;
        mem_free(pi);
        return result;
    }
    return 0;
}

void net_server_send(NetServer* server, NetClientId id, const void* buf,
                     int size, NetPriority priority)
{
    if ((id & ~NET_CLIENT_ID_ALL_BUT_FLAG) >= server->info.max_clients)
    {
        return;
    }
    SendPacketInfo* spi = mem_alloc(sizeof(SendPacketInfo) + size);
    spi->pi.packet.shead.net_head.type = NPT_S_DATA;
    mem_copy(&spi->pi.packet.payload, buf, size);
    spi->pi.size = size + sizeof(spi->pi.packet);
    spi->destination = id;
    pqueue_queue(server->send_pqueue, spi, priority);
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
