#include "client.h"
#include "utils/mem/mem.h"
#include "net/client/client.h"

typedef struct MpClient
{
    NetClient* nc;
    MpClientState state;
} MpClient;

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

bool mp_client_connection_request(MpClient* client, const char* ip,
                                  unsigned short port, const char* name)
{
    (void)name;
    if (client)
    {
        if (client->nc)
        {
            if (net_client_connection_request(client->nc, ip, port))
            {
                return true;
            }
        }
    }
    return false;
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
    if (client)
    {
        if (client->nc)
        {
            net_client_tick(client->nc);
            NetClientState ncs = net_client_get_state(client->nc);
            switch (ncs)
            {
            case NCS_UNINITED:
            {
                client->state = MPS_UNINITED;
                break;
            }
            case NCS_INITED:
            {
                client->state = MPS_INITED;
                break;
            }
            case NCS_CONNECTING:
            {
                client->state = MPS_CONNECTING;
                break;
            }
            case NCS_CONNECTED:
            {
                client->state = MPS_CONNECTED;
                break;
            }
            case NCS_CONNECTION_FAILED:
            {
                client->state = MPS_CONNECTION_FAILED;
                break;
            }
            case NCS_DISCONNECTION:
            {
                client->state = MPS_DISCONNECTED;
                break;
            }
            case NCS_DISCONNECTED:
            {
                client->state = MPS_DISCONNECTED;
                break;
            }
            }
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
