#ifndef NET_SOCKET_SOCKET_H
#define NET_SOCKET_SOCKET_H

#include <stdbool.h>

typedef struct NetSocket NetSocket;
struct sockaddr_in;

#define NET_SOCKET_MAX_MSG_SIZE   512
#define NET_SOCKET_ERROR_TIMEDOUT -2

bool net_socket_init(void);
void net_socket_destroy(void);

NetSocket* net_socket_create_socket(void);
void net_socket_destroy_socket(NetSocket* ns);
bool net_socket_bind_socket(NetSocket* ns, const char ip[],
                            unsigned short port);
int net_socket_sendto(NetSocket* ns, const struct sockaddr_in* dst,
                      const void* buf, int size);
int net_socket_recvfrom(NetSocket* ns, struct sockaddr_in* src, void* buf);

bool net_socket_set_sendto_timeout(NetSocket* ns, int timeout_ms);
bool net_socket_set_recvfrom_timeout(NetSocket* ns, int timeout_ms);

bool net_socket_set_nonblocking(NetSocket* ns, bool status);

#endif /* NET_SOCKET_SOCKET_H */
