#ifndef NET_SERVER_SERVER_H
#define NET_SERVER_SERVER_H

#include <stdbool.h>
#include "../types.h"

typedef struct NetServer NetServer;

NetServer* net_server_create(unsigned short port, NetClientId max_clients,
                             NetTime recv_timeout_ms, NetTime send_timeout_ms);
void net_server_destroy(NetServer* server);

void net_server_send(NetServer* server, NetClientId id, NetPacket* packet,
                     int size, NetPriority priority);

bool net_server_is_client_connected(NetServer* server, NetClientId id);

const NetServerInfo *net_server_get_info(NetServer* server);

bool net_server_set_recv_callback(NetServer* server,
                                  void (*callback)(NetPacket* packet,
                                                   int size));

bool net_server_set_connect_callback(NetServer* server,
                                     void (*callback)(NetClientId id));

bool net_server_set_disconnect_callback(NetServer* server,
                                        void (*callback)(NetClientId id));

#endif /* NET_SERVER_SERVER_H */
