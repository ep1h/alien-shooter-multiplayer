/**
 * @file client.h
 * @brief Client core. Contains logic for connecting to, disconnecting from, and
 * maintaining a connection with a server.
 *
 * Using this component does not spawn additional threads for sending or
 * receiving data. Instead, it uses a state machine that performs non-blocking
 * state transitions when the net_client_tick function is invoked. This design
 * enables seamless integration of send/receive logic into loop-based or
 * event-driven systems.
 */
#ifndef NET_CLIENT
#define NET_CLIENT

#include "../protocol.h"
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

/**
 * @brief Creates a new net client instance.
 *
 * @return NetClient* Pointer to the created client instance.
 * @return NULL if failed.
 */
NetClient* net_client_create(void);

/**
 * @brief Destroys net client instance.
 *
 * @param[in] client Pointer to the net client instance.
 */
void net_client_destroy(NetClient* client);

/**
 * @brief Initiates a connection request to the server.
 *
 * @param[in] client Pointer to the net client instance.
 * @param[in] ip     Server's IP address.
 * @param[in] port   Server's port.
 *
 * @return true if the client is not currently connected and the connection
 *         request was sent successfully.
 * @return false if the client is already connected or the connection request
 * failed.
 */
bool net_client_connection_request(NetClient* client, const char* ip,
                                   NetPort port);

/**
 * @brief Disconnects from server.
 *
 * @param[in] client Pointer to the net client instance.
 */
void net_client_disconnect(NetClient* client);

/**
 * @brief Processes the client's current state and transitions to the next
 * appropriate state if necessary.
 *
 * This processing includes sending all data from the send queue and receiving
 * all data into the receive queue.
 *
 * @param[in] client Pointer to the net client instance.
 */
void net_client_tick(NetClient* client);

/**
 * @brief Extracts a packet from received packets queue.
 *
 * @param[in] client Pointer to the net client instance.
 * @param[in,out] packet Pointer to allocated memory where extracted packet
 *                should be stored.
 *
 * @return Size of extracted packet (in bytes)
 * @return 0 if no packet was extracted.
 */
int net_client_dequeue_packet(NetClient* client, NetSPacket* packet);

/**
 * @brief Constructs a protocol packet with the given payload and adds it to the
 * send queue.
 *
 * Packets in the send queue will be transmitted to the server during the next
 * invocation of the net_client_tick function.
 *
 * @param[in] client   Pointer to the net client instance.
 * @param[in] buf      Pointer to the packet's payload.
 * @param[in] size     Size (in bytes) of the data in \p buf to be copied to the
 *                     packet's payload.
 * @param[in] priority Priority of the packet.
 */
void net_client_send(NetClient* client, const void* buf, int size,
                     NetPriority priority);

/**
 * @brief Retrieves the current state of the client.
 *
 * @param[in] client Pointer to the net client instance.
 *
 * @return NetClientState Current state of the client.
 */
NetClientState net_client_get_state(NetClient* client);

/**
 * @brief Returns a pointer to read-only server info structure.
 *
 * @param[in] client Pointer to the net client instance.
 *
 * @return const NetServerInfo* Pointer to read-only server info structure.
 * @return NULL if \p client is NULL.
 */
const NetServerInfo* net_client_get_server_info(NetClient* client);

/**
 * @brief Returns the ID of the client within a server if connected.
 *
 * @param[in] client Pointer to the net client instance.
 *
 * @return NetClientId Client id.
 * @return NET_CLIENT_ID_INVALID if \p client is NULL or not connected.
 */
NetClientId net_client_get_id(NetClient* client);

#endif /* NET_CLIENT */
