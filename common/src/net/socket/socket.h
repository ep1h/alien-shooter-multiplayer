/**
 * @file socket.h
 * @brief Component for initializing and using sockets.
 */
#ifndef NET_SOCKET_SOCKET_H
#define NET_SOCKET_SOCKET_H

#include <stdbool.h>

typedef struct NetSocket NetSocket;
struct sockaddr_in;

#define NET_SOCKET_MAX_MSG_SIZE   512
#define NET_SOCKET_ERROR_TIMEDOUT -2

/**
 * @brief Initialize sockets.
 *
 * @return true if success
 * @return false iffail.
 */
bool net_socket_init(void);

/**
 * @brief Destroy sockets.
 */
void net_socket_destroy(void);

/**
 * @brief Initialize a socket to use by net server/client.
 *
 * @return NetSocket* Pointer to the created socket.
 */
NetSocket* net_socket_create_socket(void);

/**
 * @brief Destroy a socket.
 */
void net_socket_destroy_socket(NetSocket* ns);

/**
 * @brief Bind a socket to an ip and port.
 *
 * @param[in] ns   Pointer to a socket object.
 * @param[in] ip   IP to bind to. If NULL, bind to INADDR_ANY.
 * @param[in] port Port to bind to.
 *
 * @return true if success.
 * @return false if fail.
 */
bool net_socket_bind_socket(NetSocket* ns, const char ip[],
                            unsigned short port);

/**
 * @brief Send a message to a destination.
 *
 * @param[in] ns   Pointer to a socket object.
 * @param[in] dst  Destination address.
 * @param[in] buf  Buffer to send.
 * @param[in] size Size of the buffer.
 *
 * @return int Positive of bytes sent.
 * @return int Negative number if fail.
 */
int net_socket_sendto(NetSocket* ns, const struct sockaddr_in* dst,
                      const void* buf, int size);

/**
 * @brief Receive a message from a socket.
 *
 * @param[in]     ns  Pointer to a socket object.
 * @param[in]     src Source address.
 * @param[in,out] buf Buffer to receive.
 *
 * @return int Positive number of bytes received.
 * @return int Negative number if fail.
 */
int net_socket_recvfrom(NetSocket* ns, struct sockaddr_in* src, void* buf);

/**
 * @brief Set send timeout for a socket.
 *
 * @param[in] ns         Pointer to a socket object.
 * @param[in] timeout_ms Timeout in milliseconds.
 *
 * @return true if success.
 * @return false if fail.
 */
bool net_socket_set_sendto_timeout(NetSocket* ns, int timeout_ms);

/**
 * @brief Set receive timeout for a socket.
 *
 * @param[in] ns         Pointer to a socket object.
 * @param[in] timeout_ms Timeout in milliseconds.
 *
 * @return true if success.
 * @return false if fail.
 */
bool net_socket_set_recvfrom_timeout(NetSocket* ns, int timeout_ms);

/**
 * @brief Set non-blocking mode for a socket.
 *
 * @param[in] ns     Pointer to a socket object.
 * @param[in] status true to set non-blocking mode, false to set blocking mode.
 *
 * @return true if success.
 * @return false if fail.
 */
bool net_socket_set_nonblocking(NetSocket* ns, bool status);

#endif /* NET_SOCKET_SOCKET_H */
