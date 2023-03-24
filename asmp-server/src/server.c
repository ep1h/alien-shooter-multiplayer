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
    NetSPacket* np = (NetSPacket*)buf;
    np->shead.net_head.type = NPT_S_DATA;
    mem_copy(np->payload, packet, size);
    net_server_send(server.ns, id, np, size + sizeof(NetSPacket), priority);
}

static void send_players_info_(NetClientId destination)
{
    const NetServerInfo* nsi = net_server_get_info(server.ns);
    unsigned char buf[1024];
    MpPacketPlayersInfo* mppi = (MpPacketPlayersInfo*)buf;
    mppi->head.type = MPT_PLAYERS_INFO;
    if (nsi)
    {
        unsigned char cur_idx = 0;
        for (NetClientId i = 0; i < nsi->max_clients; i++)
        {
            if (server.players[i].is_online)
            {
                mppi->palyers[cur_idx].id = i;
                mem_copy(&mppi->palyers[cur_idx].mp_player,
                         &server.players[i].mp_player,
                         sizeof(server.players[i].mp_player));
                ++cur_idx;
            }
        }
        mppi->players_number = cur_idx;
        send_(destination, (MpPacket*)mppi,
              sizeof(MpPacketPlayersInfo) + sizeof(MpPlayer) * cur_idx, 0);
    }
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

void on_recv_(NetCPacket* packet, int size)
{
    if (size < (int)sizeof(NetCPacket))
    {
        return;
    }
    MpPacket* mp_packet = (MpPacket*)packet->payload;
    NetClientId sender = packet->chead.sender;
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

        send_(sender, (MpPacket*)mpcr,
              sizeof(MpPacketConnectionResponse) + map_name_size, 1);
        mem_set(&server.players[sender], 0, sizeof(server.players[sender]));
        /* Store player name */
        strncpy(server.players[packet->chead.sender].mp_player.client_info.name,
                mpp->name, mpp->name_len);
        printf("store name: %s len: %d\n", mpp->name, mpp->name_len);
        server.players[sender].is_online = true;
        /* Notify other */
        send_players_info_(NET_CLIENT_ID_ALL_BUT(sender));
        break;
    }
    case MPT_PLAYERS_INFO_REQUEST:
    {
        send_players_info_(sender);
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
    mem_set(&server, 0, sizeof(server));
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
