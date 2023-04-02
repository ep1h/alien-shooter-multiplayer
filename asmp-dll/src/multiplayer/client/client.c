/**
 * @file client.c
 * @brief Top level client-side multiplayer logic implementation.
 *
 */
#include <string.h>
#include "client.h"
#include "utils/mem/mem.h"
#include "net/client/client.h"
#include "utils/time/time.h"
#include "utils/console/console.h" // TODO: Remove.


/**
 * @brief Substates related to connection state.
 */
typedef enum MpClientStateConnectedSubstate
{
    STATE_CONNECTING_SUBSTATE_IDLE = 0,
    STATE_CONNECTING_SUBSTATE_LOW_LEVEL_CONNECTING,
    STATE_CONNECTING_SUBSTATE_LOW_LEVEL_JUST_CONNECTED,
    STATE_CONNECTING_SUBSTATE_TOP_LEVEL_CONNECTING,
    STATE_CONNECTING_SUBSTATE_TOP_LEVEL_JUST_CONNECTED,
} MpClientStateConnectedSubstate;


typedef struct Player
{
    MpPlayer mp_player;
    NetTime user_sync_updated_time_ms;
    NetTime actor_sync_updated_time_ms;
} Player;

typedef struct MpClient
{
    MpClientState state;
    NetClient* nc;
    NetTime tick_time_ms;
    MpServerConfiguration server_configuration;
    Player local_player;
    void (*user_sync_callback)(MpClient* client, int id, MpUser* user);
    void (*actor_sync_callback)(MpClient* client, int id, MpActor* actor);
    void (*actor_shoot_callback)(MpClient* client, int id, float x, float y);
} MpClient;


/**
 * @brief Sends top-level client connection request and switches to the next
 *        substate.
 *
 * @param client Pointer to client instance.
 */
static bool handle_state_connecting_substate_low_level_just_connected_(
    MpClient* client);

/**
 * @brief Waits for top-level connection response.
 *
 * @param client Pointer to client instance.
 */
static bool handle_state_connecting_substate_top_level_connecting_(
    MpClient* client);

/**
 * @brief Handles connecting state.
 *
 * @param client Pointer to client instance.
 */
static void handle_state_connecting_(MpClient* client);

/**
 * @brief Handles connected state.
 *
 * @param client Pointer to client instance.
 */
static void handle_state_connected_(MpClient* client);

/**
 * @brief Dequeues and processes all packets received since last tick.
 *
 * @param client Pointer to client instance.
 */
static void process_received_packets_(MpClient* client);


static bool handle_state_connecting_substate_low_level_just_connected_(
    MpClient* client)
{
    // /* Low level client is connected, so now it is possible to get maximum
    //  * possible number of players to build players structures array */
    // if (client->remote_players)
    // {
    //     /* Clean players array from previous session */
    //     mem_free(client->remote_players);
    //     client->remote_players = 0;
    // }
    // client->remote_players =
    //     mem_alloc(sizeof(*client->remote_players) *
    //               net_client_get_server_info(client->nc)->max_clients);
    // if (!client->remote_players)
    // {
    //     return false;
    // }
    // mem_set(client->remote_players, 0,
    //         sizeof(*client->remote_players) *
    //             net_client_get_server_info(client->nc)->max_clients);

    /* Build top-level client connection request */
    uint8_t buf[sizeof(MpCPacketConnectionRequest) + MP_MAX_NAME_LEN];
    MpCPacketConnectionRequest* packet = (MpCPacketConnectionRequest*)buf;
    packet->head.type = MPT_C_CONNECTION_REQUEST;
    packet->name_len = strlen(client->local_player.mp_player.mp_user.name);
    strncpy_s(packet->name, MP_MAX_NAME_LEN,
              client->local_player.mp_player.mp_user.name, MP_MAX_NAME_LEN);
    net_client_send(client->nc, buf, sizeof(*packet) + packet->name_len, 100);
    return true;
}

static bool handle_state_connecting_substate_top_level_connecting_(
    MpClient* client)
{
    char buf[sizeof(MpSPacketConnectionResponse) + sizeof(NetSPacket)];
    NetSPacket* packet = (NetSPacket*)buf;
    int size = 0;
    while ((size = net_client_dequeue_packet(client->nc, packet)) > 0)
    {
        if (size != sizeof(*packet) + sizeof(MpSPacketConnectionResponse))
        {
            continue;
        }
        if (packet->shead.net_head.type != NPT_S_DATA)
        {
            continue;
        }
        MpPacketHead* head = (MpPacketHead*)packet->payload;
        if (head->type != MPT_S_CONENCTION_RESPONSE)
        {
            continue;
        }
        MpSPacketConnectionResponse* mp_packet =
            (MpSPacketConnectionResponse*)packet->payload;
        /* Store server configuration */
        client->server_configuration = mp_packet->server_configuration;
        return true;
    }
    return false;
}

static void handle_state_connecting_(MpClient* client)
{
    static MpClientStateConnectedSubstate connecting_substate =
        STATE_CONNECTING_SUBSTATE_IDLE;

    switch (connecting_substate)
    {
    case STATE_CONNECTING_SUBSTATE_IDLE: // TODO: Remove this state.
    {
        connecting_substate = STATE_CONNECTING_SUBSTATE_LOW_LEVEL_CONNECTING;
        break;
    }
    case STATE_CONNECTING_SUBSTATE_LOW_LEVEL_CONNECTING:
    {
        NetClientState nc_state = net_client_get_state(client->nc);
        switch (nc_state)
        {
        case NCS_CONNECTING:
        {
            /* Low level client is still connecting, just wait... */
            break;
        }
        case NCS_CONNECTED:
        {
            /* Low level client has just connected */
            /* Change substate */
            connecting_substate =
                STATE_CONNECTING_SUBSTATE_LOW_LEVEL_JUST_CONNECTED;
            break;
        }
        default:
        {
            /* Something wrong */
            /* Reset substate */
            connecting_substate = STATE_CONNECTING_SUBSTATE_IDLE;
            /* Transit to connection failed state */
            client->state = MP_CLIENT_STATE_CONNECTION_FAILED;
            break;
        }
        }
        break;
    }
    case STATE_CONNECTING_SUBSTATE_LOW_LEVEL_JUST_CONNECTED:
    {
        /* Send top level connection request */
        // TODO: Switch state to connection failed if this function returns
        //       false.
        handle_state_connecting_substate_low_level_just_connected_(client);
        /* Change substate */
        connecting_substate = STATE_CONNECTING_SUBSTATE_TOP_LEVEL_CONNECTING;
        break;
    }
    case STATE_CONNECTING_SUBSTATE_TOP_LEVEL_CONNECTING:
    {
        /* Wait for top level connection response */
        if (handle_state_connecting_substate_top_level_connecting_(client))
        {
            /* Change substate */
            connecting_substate =
                STATE_CONNECTING_SUBSTATE_TOP_LEVEL_JUST_CONNECTED;
        }
        break;
    }
    case STATE_CONNECTING_SUBSTATE_TOP_LEVEL_JUST_CONNECTED:
    {
        /* Reset connecting substate for future calls */
        connecting_substate = STATE_CONNECTING_SUBSTATE_IDLE;
        /* Transit to connected state */
        client->state = MP_CLIENT_STATE_CONNECTED;
        break;
    }
    }
}

static void handle_state_connected_(MpClient* client)
{
    if ((client->tick_time_ms -
         client->local_player.user_sync_updated_time_ms) >=
        client->server_configuration.user_sync_update_rate_ms)
    {
        /* Send actual user info */
        MpCPacketUserSync packet;
        packet.head.type = MPT_C_USER_SYNC;
        packet.mp_user = client->local_player.mp_player.mp_user;
        net_client_send(client->nc, &packet, sizeof(packet), 0);
        client->local_player.user_sync_updated_time_ms = client->tick_time_ms;
    }

    if ((client->tick_time_ms -
         client->local_player.actor_sync_updated_time_ms) >=
        client->server_configuration.actor_sync_update_rate_ms)
    {
        /* Send actual actor info */
        MpCPacketActorSync packet;
        packet.head.type = MPT_C_ACTOR_SYNC;
        mem_copy(&packet.mp_actor, &client->local_player.mp_player.mp_actor,
                 sizeof(packet.mp_actor));
        net_client_send(client->nc, &packet, sizeof(packet), 0);
        client->local_player.actor_sync_updated_time_ms = client->tick_time_ms;
    }
    process_received_packets_(client);
}

static void process_received_packets_(MpClient* client)
{
    uint8_t buf[1024]; // TODO: Use precalculated size.
    NetSPacket* packet = (NetSPacket*)buf;
    int size = 0;
    while ((size = net_client_dequeue_packet(client->nc, packet)) > 0)
    {
        if (size < (int)sizeof(*packet))
        {
            continue;
        }
        if (packet->shead.net_head.type != NPT_S_DATA)
        {
            continue;
        }
        MpPacketHead* head = (MpPacketHead*)packet->payload;
        switch (head->type)
        {
        case MPT_S_USERS_SYNC:
        {
            MpSPacketUsersSync* mp_packet =
                (MpSPacketUsersSync*)packet->payload;
            for (NetClientId i = 0; i < mp_packet->num_items; ++i)
            {
                client->user_sync_callback(client, mp_packet->items[i].id,
                                           &mp_packet->items[i].mp_user);
            }
            break;
        }
        case MPT_S_ACTORS_SYNC:
        {
            MpSPacketActorsSync* mp_packet =
                (MpSPacketActorsSync*)packet->payload;
            // console_log("MPT_S_ACTORS_SYNC. Items: %d\n",
            // mp_packet->num_items);
            for (NetClientId i = 0; i < mp_packet->num_items; ++i)
            {
                client->actor_sync_callback(client, mp_packet->items[i].id,
                                            &mp_packet->items[i].mp_actor);
                // console_log("  actor %d: %.2f %.2f\n",
                // mp_packet->items[i].id,
                //             mp_packet->items[i].mp_actor.x,
                //             mp_packet->items[i].mp_actor.y);
            }
            break;
        }
        case MPT_S_SHOOT:
        {
            client->actor_shoot_callback(
                client, ((MpSPacketShoot*)packet->payload)->player_id,
                ((MpSPacketShoot*)packet->payload)->x,
                ((MpSPacketShoot*)packet->payload)->y);
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

MpClient* mp_client_create(void)
{
    MpClient* client = mem_alloc(sizeof(*client));
    if (client)
    {
        mem_set(client, 0, sizeof(*client));
        client->nc = net_client_create();
        if (client->nc)
        {
            return client;
        }
        mem_free(client);
    }
    return 0;
}

void mp_client_destroy(MpClient* client)
{
    if (client)
    {
        if (client->nc)
        {
            net_client_destroy(client->nc);
        }
        mem_free(client);
    }
}

void mp_client_tick(MpClient* client)
{
    if (!client)
    {
        return;
    }
    client->tick_time_ms = time_get_ms();
    net_client_tick(client->nc); // TODO: Should tick only in certain states.
    switch (client->state)
    {
    case MP_CLIENT_STATE_UNINITED:
    {
        break;
    }
    case MP_CLIENT_STATE_INITED:
    {
        break;
    }
    case MP_CLIENT_STATE_CONNECTING:
    {
        handle_state_connecting_(client);
        break;
    }
    case MP_CLIENT_STATE_CONNECTED:
    {
        handle_state_connected_(client);
        break;
    }
    case MP_CLIENT_STATE_CONNECTION_FAILED:
    {
        break;
    }
    case MP_CLIENT_STATE_DISCONNECTING:
    {
        break;
    }
    }
}

bool mp_client_connection_request(MpClient* client, const char* ip,
                                  uint16_t port, const char* name)
{
    /* Check args */
    if (!client || !name || !ip)
    {
        return false;
    }
    /* Check is current client state allows to connect */
    if (client->state != MP_CLIENT_STATE_INITED &&
        client->state != MP_CLIENT_STATE_CONNECTION_FAILED)
    {
        return false;
    }
    /* Store user name */
    if (strncpy_s(client->local_player.mp_player.mp_user.name,
                  MP_MAX_NAME_LEN + 1, name, MP_MAX_NAME_LEN) != 0)
    {
        return false;
    }
    /* Send low-level client connection request */
    if (!net_client_connection_request(client->nc, ip, port))
    {
        return false;
    }
    client->state = MP_CLIENT_STATE_CONNECTING;
    return true;
}

void mp_client_disconnect(MpClient* client)
{
    if (!client)
    {
        return;
    }
    net_client_disconnect(client->nc);
    // TODO: Clear players data.
    client->state = MP_CLIENT_STATE_INITED;
}

void mp_client_set_user_sync_callback(MpClient* client,
                                      void (*callback)(MpClient* client, int id,
                                                       MpUser* user))
{
    if (client)
    {
        client->user_sync_callback = callback;
    }
}

void mp_client_set_actor_sync_callback(MpClient* client,
                                       void (*callback)(MpClient* client,
                                                        int id, MpActor* actor))
{
    if (client)
    {
        client->actor_sync_callback = callback;
    }
}

void mp_client_set_actor_shoot_callback(MpClient* client,
                                        void (*callback)(MpClient* client,
                                                         int id, float x,
                                                         float y))
{
    if (client)
    {
        client->actor_shoot_callback = callback;
    }
}

void mp_client_send_shoot(MpClient* client, float x, float y)
{
    if (client)
    {
        MpCPacketShoot packet;
        packet.head.type = MPT_C_SHOOT;
        packet.x = x;
        packet.y = y;
        net_client_send(client->nc, &packet, sizeof(packet), 100);
    }
}

MpClientState mp_client_get_state(MpClient* client)
{
    if (!client)
    {
        return MP_CLIENT_STATE_UNINITED;
    }
    return client->state;
}

const MpServerConfiguration* mp_client_get_server_configuration(
    MpClient* client)
{
    if (!client)
    {
        return 0;
    }
    return &client->server_configuration;
}

int mp_client_get_max_players_number(MpClient* client)
{
    if (!client)
    {
        return 0;
    }
    return net_client_get_server_info(client->nc)->max_clients;
}

MpPlayer* mp_client_get_local_player(MpClient* client)
{
    if (!client)
    {
        return 0;
    }
    return &client->local_player.mp_player;
}

// const MpPlayer* mp_client_get_remote_player(MpClient* client, int id)
// {
//     if (client)
//     {
//         if (id >= 0 && id <
//         net_client_get_server_info(client->nc)->max_clients)
//         {
//             if (client->remote_players[id].is_connected)
//             {
//                 return &client->remote_players[id].mp_player;
//             }
//         }
//     }
// }
