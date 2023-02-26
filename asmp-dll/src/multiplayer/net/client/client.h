#ifndef NET_CLIENT_CLIENT_H
#define NET_CLIENT_CLIENT_H

#include <stdbool.h>
#include "../types.h"

typedef struct NetClient NetClient;

NetClient* net_client_create(void);
void net_client_destroy(NetClient* client);

bool net_client_connect(NetClient* client, const char* ip, unsigned short port);
void net_client_disconnect(NetClient* client);

int net_client_dequeue_packet(NetClient* client, NetPacket* packet);
void net_client_send(NetClient* client, const void* buf, int size,
                     NetPriority priority);

bool net_client_is_connected(NetClient* client);
NetClientId net_client_get_id(NetClient* client);

#endif /* NET_CLIENT_CLIENT_H */
