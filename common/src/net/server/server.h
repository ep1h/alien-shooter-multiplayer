/**
 * @file server.h
 * @brief Server core.
 *
 * Contains logic for establishing, maintaining,
 * and terminating connectionы between UDP server and client(s).
 *
 */
#ifndef NET_SERVER_SERVER_H
#define NET_SERVER_SERVER_H

#include <stdbool.h>
#include "../protocol.h"

typedef struct NetServer NetServer;

/**
 * @brief Instantiates a server.
 *
 * @param[in] port            Port for receiving/sending data.
 * @param[in] max_clients     Maximum number of clients that can be connected to
 *                            the server simultaneously.
 * @param[in] recv_timeout_ms Time interval (in milliseconds) after which a
 *                            client will be disconnected from the server if
 *                            there is no incoming data from it.
 * @param[in] send_timeout_ms Maximum time to send data from the server to a
 *                            client (in milliseconds), after which, in case of
 *                            failure to send, the client will be disconnected
 *                            from the server.
 *
 * @return NetServer* A pointer to the created server instance for interaction
 *                      using server API.
 */
NetServer* net_server_create(unsigned short port, NetClientId max_clients,
                             NetTime recv_timeout_ms, NetTime send_timeout_ms);

/**
 * @brief Destroys a previously created server.
 *
 * @param server Server instance pointer.
 */
void net_server_destroy(NetServer* server);

/**
 * @brief Executes server logic.
 *
 * Places all packets received since the last call to this function into
 * received message queue.
 * Sends all packets from send queue.
 * Manages clients: kick delayed.
 *
 * @param[in] server Server instance pointer.
 */
void net_server_tick(NetServer* server);

/**
 * @brief Extracts data from received packets queue.
 *
 * @param[in]  server Server instance pointer.
 * @param[out] packet Pointer to a buffer where dequeued the packet should be
 *                      written.
 * @return int  Size of dequeued packet.
 */
int net_server_dequeue_packet(NetServer* server, NetCPacket* packet);

/**
 * @brief Queues data from \p buf buffer to send packet queue.
 *
 * All packets from this queue will be sent during the next call to the
 * net_server_tick function.
 *
 * @param[in] server   Server instance pointer.
 * @param[in] id       Destination client id to which the data should be sent.
 * @param[in] buf      Pointer to data to be queued in send queue.
 * @param[in] size     Size of data to be queued in send queue.
 * @param[in] priority Priority with which this data should be sent during the
 *                     next call to net_server_tick function.
 */
void net_server_send(NetServer* server, NetClientId destination,
                     const void* buf, int size, NetPriority priority);

/**
 * @brief Checks online status of a specific client.
 *
 * @param[in] server Server instance pointer.
 * @param[in] id     Id of a client whose online status is to be obtained.
 *
 * @return true if client with \p id id is connected.
 * @return false if client with \p id id is not connected or client id is wrong.
 */
bool net_server_is_client_connected(NetServer* server, NetClientId id);


/**
 * @brief Returns a read-only pointer to a structure with the server's
 * configuration.
 *
 * @param[in] server Server instance pointer.
 *
 * @return const NetServerInfo* read-only pointer to a structure with the
 * server's configuration.
 */
const NetServerInfo* net_server_get_info(NetServer* server);

#endif /* NET_SERVER_SERVER_H */
