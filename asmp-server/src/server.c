#include <stdio.h>
#include <string.h>
#include "server.h"
#include "utils/mem/mem.h"
#include "net/server/server.h"
#include "multiplayer_protocol.h"
#include "utils/time/time.h"

#define ACTORS_INFO_UPDATE_RATE_MS 60

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
    NetTime prev_actors_info_updated_time_ms;
} MpServer;

static void send_actors_info_(MpServer* server);

static void send_actors_info_(MpServer* server)
{
    for (int i = 0; i < net_server_get_info(server->ns)->max_clients; i++)
    {
        if (!server->players[i].is_online)
        {
            continue;
        }
        NetByte buf[1024] = {0};
        MpPacketActorsInfo* p = (MpPacketActorsInfo*)buf;
        p->head.type = MPT_ACTORS_INFO;

        /* Build packet for 'i' player */
        for (int j = 0; j < net_server_get_info(server->ns)->max_clients; j++)
        {
            if (server->players[j].is_online && i != j)
            {
                // printf("build packed for %d: insert info about %d\n", i, j);
                MpInfoWrapper* dst_iw = (MpInfoWrapper*)((
                    ((NetByte*)p->info_wrapper) +
                    p->actors_number *
                        (sizeof(MpInfoWrapper) + sizeof(MpActorInfo))));
                dst_iw->id = j;
                mem_copy(&dst_iw->payload,
                         &server->players[j].mp_player.actor_info,
                         sizeof(server->players[j].mp_player.actor_info));
                p->actors_number++;
            }
        }
        int size =
            sizeof(MpPacketActorsInfo) +
            (sizeof(MpInfoWrapper) + sizeof(MpActorInfo)) * p->actors_number;
        net_server_send(server->ns, i, p, size, 0);
    }
}

static void send_players_info_(MpServer* server, NetClientId destination)
{
    const NetServerInfo* nsi = net_server_get_info(server->ns);
    unsigned char buf[1024];
    MpPacketPlayersInfo* mppi = (MpPacketPlayersInfo*)buf;
    mppi->head.type = MPT_PLAYERS_INFO;
    if (nsi)
    {
        unsigned char cur_idx = 0;
        for (NetClientId i = 0; i < nsi->max_clients; i++)
        {
            if (server->players[i].is_online)
            {
                mppi->palyers[cur_idx].id = i;
                mem_copy(&mppi->palyers[cur_idx].mp_player,
                         &server->players[i].mp_player,
                         sizeof(server->players[i].mp_player));
                ++cur_idx;
            }
        }
        mppi->players_number = cur_idx;
        net_server_send(
            server->ns, destination, (MpPacket*)mppi,
            sizeof(MpPacketPlayersInfo) + sizeof(MpPlayer) * cur_idx, 0);
    }
}

static void on_recv_(MpServer* server, NetCPacket* packet, int size)
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

        char buf[sizeof(MpPacketConnectionResponse) + 255];
        MpPacketConnectionResponse* mpcr = (MpPacketConnectionResponse*)buf;
        int map_name_size = strlen(server->map_name) + 1;
        mpcr->head.type = MPT_CONNECTION_RESPONSE;
        mpcr->server_info.map_name_len = map_name_size - 1;
        strcpy(mpcr->server_info.map_name, server->map_name);

        net_server_send(server->ns, sender, (MpPacket*)mpcr,
                        sizeof(MpPacketConnectionResponse) + map_name_size, 1);
        mem_set(&server->players[sender], 0, sizeof(server->players[sender]));
        /* Store player name */
        strncpy(
            server->players[packet->chead.sender].mp_player.client_info.name,
            mpp->name, mpp->name_len);
        printf("store name: %s len: %d\n", mpp->name, mpp->name_len);
        server->players[sender].is_online = true;
        /* Notify other */
        send_players_info_(server, NET_CLIENT_ID_ALL_BUT(sender));
        break;
    }
    case MPT_PLAYERS_INFO_REQUEST:
    {
        send_players_info_(server, sender);
        break;
    }
    case MPT_ACTOR_INFO:
    {
        /* Store actor info */
        MpPacketActorInfo* mpp = (MpPacketActorInfo*)(mp_packet);
        mem_copy(&server->players[sender].mp_player.actor_info,
                 &mpp->mp_actor_info, sizeof(mpp->mp_actor_info));
        break;
    }
    default:
    {
        break;
    }
    }
}

MpServer* mp_server_create(unsigned short port, int max_clients)
{
    MpServer* server = mem_alloc(sizeof(*server));
    if (server)
    {
        mem_set(server, 0, sizeof(*server));
        server->ns = net_server_create(port, max_clients, 5000, 5000);
        if (server->ns)
        {
            strcpy(server->map_name, "maps\\Level_01.map");
            server->players =
                mem_alloc(sizeof(server->players[0]) * max_clients);
            if (server->players)
            {
                server->prev_actors_info_updated_time_ms = time_get_ms();
                return server;
            }
            net_server_destroy(server->ns);
        }
        mem_free(server);
    }
    return 0;
}

void mp_server_destroy(MpServer* server)
{
    if (server)
    {
        if (server->ns)
        {
            net_server_destroy(server->ns);
        }
        if(server->players)
        {
            mem_free(server->players);
        }
        mem_free(server);
    }
}

void mp_server_tick(MpServer* server)
{
    net_server_tick(server->ns);
    char buf[2048];
    int size;
    while ((size = net_server_dequeue_packet(server->ns, (NetCPacket*)buf)))
    {
        on_recv_(server, (NetCPacket*)buf, size);
    }
    NetTime time = time_get_ms();
    if ((time - server->prev_actors_info_updated_time_ms) >=
        ACTORS_INFO_UPDATE_RATE_MS)
    {
        send_actors_info_(server);
        server->prev_actors_info_updated_time_ms = time;
    }
}
