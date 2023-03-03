#ifndef NET_SERVER_SERVER_H
#define NET_SERVER_SERVER_H

#include <stdbool.h>
#include "../types.h"

typedef struct NetServer NetServer;

NetServer* net_server_create(unsigned short port, NetClientId max_clients,
                             NetTime recv_timeout_ms, NetTime send_timeout_ms);
void net_server_destroy(NetServer* server);

int net_server_dequeue_packet(NetServer* server, NetPacket* packet);

void net_server_send(NetServer* server, NetClientId id, NetPacket* packet,
                     int size, NetPriority priority);

bool net_server_is_client_connected(NetServer* server, NetClientId id);

const NetServerInfo *net_server_get_info(NetServer* server);

#endif /* NET_SERVER_SERVER_H */
