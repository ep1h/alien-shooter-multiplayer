#include <string.h>
#include "client.h"
#include "utils/mem/mem.h"
#include "net/client/client.h"
#include "multiplayer_protocol.h"

typedef struct MpClient
{
    NetClient* nc;
    MpClientState state;
    char server_ip[16];
    unsigned short server_port;
    char player_name[MP_MAX_NAME_LEN];
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
        cs = CS_TOP_LEVEL_CONNECTING;

        client->state = MPS_CONNECTED; // TODO: Remove this line.
        cs = CS_IDLE;
        break;
    }
    case CS_TOP_LEVEL_CONNECTING: /* Wait for top level to be connected */
    {

        break;
    }
    case CS_TOP_LEVEL_CONNECTED:
    {
        client->state = MPS_CONNECTED;
        break;
    }
    }
}

void handle_connected_(MpClient* client)
{
    (void)client;
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
