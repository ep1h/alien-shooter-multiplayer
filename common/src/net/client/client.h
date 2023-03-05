#ifndef NET_CLIENT
#define NET_CLIENT

#include "../types.h"

#include <stdbool.h>

typedef struct NetClient NetClient;

typedef enum NetClientState
{
    NCS_UNINITED = 0,
    NCS_INITED,
    NCS_CONNECTING,
    NCS_CONNECTED,
    NCS_CONNECTION_FAILED,
    NCS_DISCONNECTION,
    NCS_DISCONNECTED,
} NetClientState;

NetClient* net_client_create(void);
void net_client_destroy(NetClient* client);

bool net_client_connection_request(NetClient* client, const char* ip,
                                   NetPort port);
void net_client_disconnect(NetClient* client);

void net_client_tick(NetClient* client);

int net_client_dequeue_packet(NetClient* client, NetPacket* packet);
void net_client_send(NetClient* client, const void* buf, int size,
                     NetPriority priority);

NetClientState net_client_get_state(NetClient* client);

bool net_client_get_server_info(NetClient* client,
                                NetServerInfo* const out_server_info);

#endif /* NET_CLIENT */
