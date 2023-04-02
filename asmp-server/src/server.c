/**
 * @file server.c
 * @brief Top level server-side multiplayer logic.
 *
 */
#include <string.h>
#include "server.h"
#include "multiplayer_protocol.h"
#include "net/server/server.h"
#include "utils/mem/mem.h"
#include "utils/time/time.h"
#include <stdio.h> // TODO: Remove.

typedef struct Player
{
    MpPlayer player;
    bool is_connected;
    NetTime user_sync_updatd_time_ms;
    NetTime actor_sync_updated_time_ms;
} Player;

typedef struct MpServer
{
    NetServer* ns;
    MpServerConfiguration server_configuration;
    NetTime tick_time_ms;
    Player* players;
    NetTime prev_users_sync_sent_time_ms;
    NetTime prev_actors_sync_sent_time_ms;
} MpServer;


/**
 * @brief process connection request packet.
 *
 * @param server A pointer to server instance.
 * @param sender Sender low-level client id.
 * @param packet A pointer to connection request packet.
 */
static void process_connection_request_(MpServer* server, NetClientId sender,
                                        MpCPacketConnectionRequest* packet);

/**
 * @brief Process packets received since last tick.
 *
 * @param server A pointer to server instance.
 */
static void process_received_packets_(MpServer* server);


/**
 * @brief Builds and sends users sync packets to all connected clients.
 *
 * @param server A pointer to server instance.
 * Each client will receive user data about other connected clients.
 */
static void send_users_sync_(MpServer* server);

/**
 * @brief Builds and sends actors sync packets to all connected clients.
 *
 * Each client will receive information about other connected clients.
 */
static void send_actors_sync_(MpServer* server);


static void process_connection_request_(MpServer* server, NetClientId sender,
                                        MpCPacketConnectionRequest* packet)
{
    printf("Connection request from %d\n", sender);
    /* Store player name */
    strncpy_s(server->players[sender].player.mp_user.name, MP_MAX_NAME_LEN + 1,
              packet->name, MP_MAX_NAME_LEN);
    /* Mark as connected */
    server->players[sender].is_connected = true;
    /* Send connection response */
    MpSPacketConnectionResponse response;
    response.head.type = MPT_S_CONENCTION_RESPONSE;
    response.server_configuration = server->server_configuration;
    net_server_send(server->ns, sender, &response, sizeof(response), 0);
}

static void process_received_packets_(MpServer* server)
{
    char buf[2048];
    int size;
    while ((size = net_server_dequeue_packet(server->ns, (NetCPacket*)buf)))
    {
        if (size < (int)sizeof(NetCPacket))
        {
            continue;
        }
        NetCPacket* packet = (NetCPacket*)buf;
        if (packet->chead.net_head.type != NPT_C_DATA)
        {
            continue;
        }
        MpPacketHead* mp_head = (MpPacketHead*)packet->payload;
        switch (mp_head->type)
        {
        case MPT_C_CONNECTION_REQUEST:
        {
            process_connection_request_(
                server, packet->chead.sender,
                (MpCPacketConnectionRequest*)packet->payload);
            break;
        }
        case MPT_C_USER_SYNC:
        {
            /* Update user in server struct */
            server->players[packet->chead.sender].player.mp_user =
                ((MpCPacketUserSync*)packet->payload)->mp_user;
            server->players[packet->chead.sender].user_sync_updatd_time_ms =
                server->tick_time_ms;
            break;
        }
        case MPT_C_ACTOR_SYNC:
        {
            // printf("MPT_C_ACTOR_SYNC from %d | %.2f %.2f\n",
            //        packet->chead.sender,
            //        ((MpCPacketActorSync*)packet->payload)->mp_actor.x,
            //        ((MpCPacketActorSync*)packet->payload)->mp_actor.y);
            /* Update actor in server struct */
            server->players[packet->chead.sender].player.mp_actor =
                ((MpCPacketActorSync*)packet->payload)->mp_actor;
            server->players[packet->chead.sender].actor_sync_updated_time_ms =
                server->tick_time_ms;
            break;
        }
        case MPT_C_SHOOT:
        {
            /* Build and send shoot packet to all other clients */
            MpSPacketShoot msps;
            msps.head.type = MPT_S_SHOOT;
            msps.player_id = packet->chead.sender;
            msps.x = ((MpCPacketShoot*)packet->payload)->x;
            msps.y = ((MpCPacketShoot*)packet->payload)->y;
            net_server_send(server->ns,
                            NET_CLIENT_ID_ALL_BUT(packet->chead.sender), &msps,
                            sizeof(msps), 100);
            break;
        }
        default:
        {
            break;
        }
        }
    }
}
static void send_users_sync_(MpServer* server)
{
    uint8_t buf[1024]; // TODO: Use precalculated size.
    MpSPacketUsersSync* packet = (MpSPacketUsersSync*)buf;
    packet->head.type = MPT_S_USERS_SYNC;


    for (NetClientId destination = 0;
         destination < net_server_get_info(server->ns)->max_clients;
         destination++)
    {
        /* Build actors info sync packet only for connected players */
        if (!server->players[destination].is_connected)
        {
            continue;
        }

        /* Reset num_items for the player */
        packet->num_items = 0;

        /* Build actors sync packet for player with 'destination' id */
        for (NetClientId i = 0;
             i < net_server_get_info(server->ns)->max_clients; i++)
        {
            /* Ignore disconnected players */
            if (!server->players[i].is_connected)
            {
                continue;
            }
            /* Ignore player who will receive this packet */
            if (i == destination)
            {
                continue;
            }
            /* Ignore players who did not send user sync yet */
            if (server->players[i].user_sync_updatd_time_ms == 0)
            {
                continue;
            }
            // TODO: Ignore players who did not send user sync for a long
            //       time.

            /* Add player's actor info to packet */
            packet->items[packet->num_items].id = i;
            packet->items[packet->num_items].mp_user =
                server->players[i].player.mp_user;
            packet->num_items++;
        }
        /* Send the packet */
        net_server_send(
            server->ns, destination, packet,
            sizeof(*packet) + sizeof(packet->items[0]) * packet->num_items, 0);
    }
}

static void send_actors_sync_(MpServer* server)
{
    uint8_t buf[1024]; // TODO: Use precalculated size.
    MpSPacketActorsSync* packet = (MpSPacketActorsSync*)buf;
    packet->head.type = MPT_S_ACTORS_SYNC;


    for (NetClientId destination = 0;
         destination < net_server_get_info(server->ns)->max_clients;
         destination++)
    {
        /* Build actors info sync packet only for connected players */
        if (!server->players[destination].is_connected)
        {
            continue;
        }

        /* Reset num_items for the player */
        packet->num_items = 0;

        /* Build actors sync packet for player with 'destination' id */
        for (NetClientId i = 0;
             i < net_server_get_info(server->ns)->max_clients; i++)
        {
            /* Ignore disconnected players */
            if (!server->players[i].is_connected)
            {
                continue;
            }
            /* Ignore player who will receive this packet */
            if (i == destination)
            {
                continue;
            }
            /* Ignore players who did not send actor sync yet */
            if (server->players[i].actor_sync_updated_time_ms == 0)
            {
                continue;
            }
            // TODO: Ignore players who did not send actor sync for a long
            //       time.

            /* Add player's actor info to packet */
            packet->items[packet->num_items].id = i;
            packet->items[packet->num_items].mp_actor =
                server->players[i].player.mp_actor;
            packet->num_items++;
        }
        /* Send the packet */
        net_server_send(
            server->ns, destination, packet,
            sizeof(*packet) + sizeof(packet->items[0]) * packet->num_items, 0);
    }
}

MpServer* mp_server_create(unsigned short port, int max_clients)
{
    MpServer* server = mem_alloc(sizeof(*server));
    if (server)
    {
        mem_set(server, 0, sizeof(*server));
        server->ns = net_server_create(port, max_clients, MP_RECV_TIMEOUT_MS,
                                       MP_SEND_TIMEOUT_MS);
        if (server->ns)
        {
            server->players = mem_alloc(sizeof(*server->players) * max_clients);
            if (server->players)
            {
                mem_set(server->players, 0,
                        sizeof(*server->players) * max_clients);
                strcpy_s(server->server_configuration.map_name,
                         MP_MAX_MAP_NAME_LEN, "maps\\Level_01.map");
                server->server_configuration.user_sync_update_rate_ms =
                    MP_USER_SYNC_UPDATE_RATE_MS;
                server->server_configuration.actor_sync_update_rate_ms =
                    MP_ACTOR_SYNC_UPDATE_RATE_MS;
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
        mem_free(server);
    }
}

void mp_server_tick(MpServer* server)
{
    if (!server)
    {
        return;
    }
    server->tick_time_ms = time_get_ms();
    net_server_tick(server->ns);
    process_received_packets_(server);

    if ((server->tick_time_ms - server->prev_users_sync_sent_time_ms) >=
        server->server_configuration.user_sync_update_rate_ms)
    {
        send_users_sync_(server);
        server->prev_users_sync_sent_time_ms = server->tick_time_ms;
    }

    if ((server->tick_time_ms - server->prev_actors_sync_sent_time_ms) >=
        server->server_configuration.actor_sync_update_rate_ms)
    {
        send_actors_sync_(server);
        server->prev_actors_sync_sent_time_ms = server->tick_time_ms;
    }
}
