#include <stdio.h>
#include <string.h>
#include "server.h"
#include "utils/mem/mem.h"
#include "net/server/server.h"
#include "multiplayer_protocol.h"

typedef struct Player
{
    MpPlayer mp_player;
    bool is_online;
} Player;

typedef struct MpServer
{
    NetServer* ns;
    char map_name[255];
    Player* players;
} MpServer;

static MpServer server = {0};

static void send_(NetClientId id, MpPacket* packet, int size,
                  NetPriority priority)
{
    char buf[1024];
    NetPacket* np = (NetPacket*)buf;
    np->head.type = NPT_DATA;
    mem_copy(np->payload, packet, size);
    net_server_send(server.ns, id, np, size + sizeof(NetPacket), priority);
}

void on_connect_(NetClientId id)
{
    printf("mp connect %d\n", id);
}

void on_disconnect_(NetClientId id)
{
    printf("mp disconnect %d\n", id);
    mem_set(&server.players[id], 0, sizeof(server.players[id]));
}

void on_recv_(NetPacket* packet, int size)
{
    (void)size;
    MpPacket* mp_packet = (MpPacket*)packet->payload;
    switch (mp_packet->head.type)
    {
    case MPT_CONNECTION_REQUEST:
    {
        MpPacketConnectionRequest* mpp = (MpPacketConnectionRequest*)mp_packet;

        char buf[sizeof(MpPacketConnectionResponse) + sizeof(server)];
        MpPacketConnectionResponse* mpcr = (MpPacketConnectionResponse*)buf;
        int map_name_size = strlen(server.map_name) + 1;
        mpcr->head.type = MPT_CONNECTION_RESPONSE;
        mpcr->server_info.map_name_len = map_name_size - 1;
        strcpy(mpcr->server_info.map_name, server.map_name);

        send_(packet->head.sender, (MpPacket*)mpcr,
              sizeof(MpPacketConnectionResponse) + map_name_size, 0);
        mem_set(&server.players[packet->head.sender], 0,
                sizeof(server.players[packet->head.sender]));

        /* Store player name */
        strncpy(server.players[packet->head.sender].mp_player.client_info.name,
               mpp->name, mpp->name_len);

        server.players[packet->head.sender].is_online = true;
        break;
    }
    default:
    {
        break;
    }
    }
}

bool mp_server_init(unsigned short port, int max_clients)
{
    server.ns = net_server_create(port, max_clients, 5000, 5000);
    if (server.ns)
    {
        net_server_set_recv_callback(server.ns, on_recv_);
        net_server_set_connect_callback(server.ns, on_connect_);
        net_server_set_disconnect_callback(server.ns, on_disconnect_);
        strcpy(server.map_name, "maps\\survive_01.map");
        server.players = mem_alloc(sizeof(server.players[0]) * max_clients);
        if (server.players)
        {
            return true;
        }
    }
    return false;
}

void mp_server_destroy(void)
{
    if (server.ns)
    {
        net_server_destroy(server.ns);
    }
}
