#include <string.h>
#include "client.h"
#include "utils/mem/mem.h"
#include "net/client/client.h"
#include "multiplayer_protocol.h"
#include "utils/console/console.h"
#include "utils/time/time.h"

#define REQUEST_DELAY_MS 1000

typedef struct RequestInfo
{
    NetTime send_time_ms;
    NetTime recv_time_ms;
} RequestInfo;

typedef struct Player
{
    bool is_connected;
    MpPlayer mp_player;
} Player;

typedef struct MpClient
{
    NetClient* nc;
    MpClientState state;
    char server_ip[16];
    unsigned short server_port;
    char player_name[MP_MAX_NAME_LEN];
    Player* players;
    RequestInfo client_info_request;
} MpClient;

typedef enum ConnectionSubstate
{
    CS_IDLE = 0,
    CS_LOW_LEVEL_CONNECTING = 1,
    CS_LOW_LEVEL_CONNECTED = 2,
    CS_TOP_LEVEL_CONNECTING = 3,
    CS_TOP_LEVEL_CONNECTED = 4
} ConnectionSubstate;

void handle_uninited_(MpClient* client);
void handle_inited_(MpClient* client);
void handle_connecting_(MpClient* client);
void handle_connected_(MpClient* client);
void handle_connection_failed_(MpClient* client);
void handle_disconnection_(MpClient* client);
void handle_disconnected_(MpClient* client);

void handle_uninited_(MpClient* client)
{
    (void)client;
}

void handle_inited_(MpClient* client)
{
    (void)client;
}

void handle_connecting_(MpClient* client)
{
    (void)client;
    static ConnectionSubstate cs = CS_IDLE;
    switch (cs)
    {
    case CS_IDLE:
    {
        /* Send low level connection request */
        if (net_client_connection_request(client->nc, client->server_ip,
                                          client->server_port))
        {
            cs = CS_LOW_LEVEL_CONNECTING;
        }
        else
        {
            client->state = MPS_CONNECTION_FAILED;
        }
        break;
    }
    case CS_LOW_LEVEL_CONNECTING: /* Wait for low level to be connected */
    {
        NetClientState ncs = net_client_get_state(client->nc);
        switch (ncs)
        {
        case NCS_CONNECTING:
        {
            /* Just wait */
            break;
        }
        case NCS_CONNECTED:
        {
            cs = CS_LOW_LEVEL_CONNECTED;
            break;
        }
        case NCS_CONNECTION_FAILED:
        default:
        {
            client->state = MPS_CONNECTION_FAILED;
            cs = CS_IDLE;
            break;
        }
        }
        break;
    }
    case CS_LOW_LEVEL_CONNECTED:
    {
        /* Send top level conenction request */
        int name_len = strlen(client->player_name);
        unsigned char
            mpcr_buf[sizeof(MpPacketConnectionRequest) + MP_MAX_NAME_LEN + 1];
        MpPacketConnectionRequest* mpcr = (MpPacketConnectionRequest*)mpcr_buf;
        mpcr->head.type = MPT_CONNECTION_REQUEST;
        mpcr->name_len = name_len;
        strcpy(mpcr->name, client->player_name);
        net_client_send(client->nc, mpcr,
                        sizeof(MpPacketConnectionRequest) + name_len + 1, 1);
        cs = CS_TOP_LEVEL_CONNECTING;
        break;
    }
    case CS_TOP_LEVEL_CONNECTING:
    {
        /* Wait for top level to be connected */
        char buf[1024];
        NetPacket* np = (NetPacket*)buf;
        for (int size = net_client_dequeue_packet(client->nc, np); size;
             size = net_client_dequeue_packet(client->nc, np))
        {
            MpPacketConnectionResponse* mpcr =
                (MpPacketConnectionResponse*)np->payload;
            if (mpcr->head.type == MPT_CONNECTION_RESPONSE)
            {
                console_log("recv connection response. Map: %s\n",
                            mpcr->server_info.map_name);
                /* Store top level server info here (map name, etc...) */
                const NetServerInfo* nsi =
                    net_client_get_server_info(client->nc);
                if (nsi)
                {
                    client->players = mem_alloc(sizeof(client->players[0]) *
                                                nsi->max_clients);
                    if (client->players)
                    {
                        cs = CS_TOP_LEVEL_CONNECTED;
                        break;
                    }
                    client->state = MPS_CONNECTION_FAILED;
                    break;
                }
            }
        }
        break;
    }
    case CS_TOP_LEVEL_CONNECTED:
    {
        console_log("CS_TOP_LEVEL_CONNECTED\n");
        client->state = MPS_CONNECTED;
        cs = CS_IDLE;
        break;
    }
    }
}

void handle_connected_(MpClient* client)
{
    /* Request data */
    NetTime time = (NetTime)time_get_ms();
    if ((((time - client->client_info_request.recv_time_ms) > 10000) &&
         ((time - client->client_info_request.send_time_ms) >
          REQUEST_DELAY_MS)) ||
        (client->client_info_request.send_time_ms == 0))
    {
        console_log("send MpPacketPlayersInfoRequest\n");
        MpPacketPlayersInfoRequest mppir;
        mppir.head.type = MPT_PLAYERS_INFO_REQUEST;
        net_client_send(client->nc, &mppir, sizeof(mppir), 5);
        client->client_info_request.send_time_ms = time;
    }

    /* Dequeue all received packets */
    char buf[1024];
    NetPacket* p = (NetPacket*)buf;
    for (int size = net_client_dequeue_packet(client->nc, p); size > 0;
         size = net_client_dequeue_packet(client->nc, p))
    {
        // NetClientId sender = p->head.sender;
        if (p->head.type == NPT_DATA)
        {
            MpPacket* mp = (MpPacket*)p->payload;
            switch (mp->head.type)
            {
            case MPT_PLAYERS_INFO:
            {
                console_log("MPT_PLAYERS_INFO\n");
                client->client_info_request.recv_time_ms = time;
                MpPacketPlayersInfo* mppi = (MpPacketPlayersInfo*)mp;

                const NetServerInfo* nsi =
                    net_client_get_server_info(client->nc);
                if (nsi)
                {
                    for (unsigned int i = 0; i < nsi->max_clients; i++)
                    {
                        // TODO: The next code may be unsafe.
                        client->players[i].is_connected = false;
                    }
                    for (unsigned char i = 0; i < mppi->players_number; i++)
                    {
                        console_log("  from %s[%d]\n",
                                    mppi->palyers[i].mp_player.client_info.name,
                                    mppi->palyers[i].id);
                        mem_copy(
                            &client->players[mppi->palyers[i].id].mp_player,
                            &mppi->palyers[i].mp_player,
                            sizeof(mppi->palyers[i].mp_player));
                        client->players[mppi->palyers[i].id].is_connected =
                            true;
                    }
                    break;
                }
            }
            default:
            {
                break;
            }
            }
        }
    }
}

void handle_connection_failed_(MpClient* client)
{
    (void)client;
}

void handle_disconnection_(MpClient* client)
{
    (void)client;
}

void handle_disconnected_(MpClient* client)
{
    (void)client;
}

MpClient* mp_client_create(void)
{
    MpClient* client = mem_alloc(sizeof(MpClient));
    if (client)
    {
        mem_set(client, 0, sizeof(*client));
        if (client)
        {
            client->nc = net_client_create();
            if (client->nc)
            {
                return client;
            }
            mem_free(client);
        }
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

void mp_client_connection_request(MpClient* client, const char* ip,
                                  unsigned short port, const char* name)
{
    if (client)
    {
        strncpy_s(client->server_ip, sizeof(client->server_ip), ip,
                  sizeof(client->server_ip));
        client->server_port = port;
        strncpy_s(client->player_name, sizeof(client->player_name), name,
                  sizeof(client->player_name));
        client->state = MPS_CONNECTING;
    }
}

void mp_client_disconnect(MpClient* client)
{
    if (client)
    {
        net_client_disconnect(client->nc);
    }
}

void mp_client_tick(MpClient* client)
{
    if (!client || !client->nc)
    {
        return;
    }
    net_client_tick(client->nc);
    switch (client->state)
    {
    case MPS_UNINITED:
    {
        handle_uninited_(client);
        break;
    }
    case MPS_INITED:
    {
        handle_inited_(client);
        break;
    }
    case MPS_CONNECTING:
    {
        handle_connecting_(client);
        break;
    }
    case MPS_CONNECTED:
    {
        handle_connected_(client);
        break;
    }
    case MPS_CONNECTION_FAILED:
    {
        handle_connection_failed_(client);
        break;
    }
    case MPS_DISCONNECTION:
    {
        handle_disconnection_(client);
        break;
    }
    case MPS_DISCONNECTED:
    {
        handle_disconnected_(client);
        break;
    }
    }
}

MpClientState mp_client_get_state(MpClient* client)
{
    if (client)
    {
        return client->state;
    }
    return MPS_UNINITED;
}
