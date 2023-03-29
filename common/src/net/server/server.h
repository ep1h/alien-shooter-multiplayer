/**
 * @file server.h
 * @brief Server core.
 *
 * Contains logic for establishing, maintaining,
 * and terminating connections between the UDP server and its client(s).
 */
#ifndef NET_SERVER_SERVER_H
#define NET_SERVER_SERVER_H

#include <stdbool.h>
#include "../protocol.h"

typedef struct NetServer NetServer;

/**
 * @brief Instantiates a server.
 *
 * @param[in] port            Port for receiving and sending data.
 * @param[in] max_clients     Maximum number of clients that can connect to
 *                            the server simultaneously.
 * @param[in] recv_timeout_ms Time interval (in milliseconds) after which a
 *                            client will be disconnected if no incoming data is
 *                            received from it.
 * @param[in] send_timeout_ms Maximum time (in milliseconds) to send data from
 *                            the server to a client. If sending fails within
 *                            this interval, the client will be disconnected.
 *
 * @return NetServer* A pointer to the created server instance for interactions
 *                    using the server API.
 */
NetServer* net_server_create(unsigned short port, NetClientId max_clients,
                             NetTime recv_timeout_ms, NetTime send_timeout_ms);

/**
 * @brief Destroys a previously instantiated server.
 *
 * @param server Pointer to the server instance to be destroyed.
 */
void net_server_destroy(NetServer* server);

/**
 * @brief Processes server logic.
 *
 * Puts all packets received since the last call into the received message
 * queue.
 * Sends all packets from the send queue.
 * Manages clients by kicking those that are delayed.
 *
 * @param[in] server Pointer to the server instance.
 */
void net_server_tick(NetServer* server);

/**
 * @brief Dequeues data from the received packets queue.
 *
 * @param[in]  server Pointer to the server instance.
 * @param[out] packet Pointer to a buffer where the dequeued packet will be
 *                    stored.
 * @return int Size of the dequeued packet.
 */
int net_server_dequeue_packet(NetServer* server, NetCPacket* packet);

/**
 * @brief Adds data from the \p buf buffer to the send packet queue.
 *
 * All packets in this queue will be sent during the next call to the
 * net_server_tick function.
 *
 * @param[in] server      Pointer to the server instance.
 * @param[in] destination Destination client ID to which the data will be sent.
 * @param[in] buf         Pointer to the data to be added to the send queue.
 * @param[in] size        Size of the data to be added to the send queue.
 * @param[in] priority    Priority with which this data will be sent during
 *                        the next call to the \p net_server_tick function.
 */
void net_server_send(NetServer* server, NetClientId destination,
                     const void* buf, int size, NetPriority priority);

/**
 * @brief Checks the online status of a specific client.
 *
 * @param[in] server Pointer to the server instance.
 * @param[in] id     ID of the client whose online status is to be checked.
 *
 * @return true if the client with the given \p id is connected.
 * @return false if the client with the given \p id is not connected or the
 *         client ID is invalid.
 */
bool net_server_is_client_connected(NetServer* server, NetClientId id);

/**
 * @brief Returns a read-only pointer to a structure containing the server's
 * configuration details.
 *
 * @param[in] server Pointer to the server instance.
 *
 * @return const NetServerInfo* A read-only pointer to a structure containing
 * the configuration details of the server to which the client is connected.
 */
const NetServerInfo* net_server_get_info(NetServer* server);

#endif /* NET_SERVER_SERVER_H */
