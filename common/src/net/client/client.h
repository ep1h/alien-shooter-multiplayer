/**
 * @file client.h
 * @brief Client core. Contains logic for connecting/disconnecting/maintaining a
 *        connection with a server.
 * 
 * Usage of this component does not create additional
 * threads for receiving/sending data. Instead, a state machine is used that
 * performs non-blocking processing of states when net_client_tick function is
 * called. This allows elegantly embed send/receive logic into any loop-based or
 * event-based systems.
 *
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
 * @return NetClient* Pointer to created client instance.
 * @return NULL if failed.
 */
NetClient* net_client_create(void);

/**
 * @brief Destroys net client instance.
 *
 * @param[in] client Pointer to net client instance.
 */
void net_client_destroy(NetClient* client);

/**
 * @brief Sends connection request to server.
 *
 * @param[in] client Pointer to net client instance.
 * @param[in] ip     Server IP address.
 * @param[in] port   Server port.
 *
 * @return true if the client has not yet been connected and connection request
 *         has been sent successfully.
 * @return false if client is already connected or connection request has not
 *         been sent.
 */
bool net_client_connection_request(NetClient* client, const char* ip,
                                   NetPort port);

/**
 * @brief Disconnects from server.
 *
 * @param[in] client Pointer to net client instance.
 */
void net_client_disconnect(NetClient* client);

/**
 * @brief Processes the client depending on its state. Switches the state.
 *
 * @param[in] client Pointer to net client instance.
 */
void net_client_tick(NetClient* client);

/**
 * @brief Extracts a packet from received packets queue.
 *
 * @param[in]     client Pointer to net client instance.
 * @param[in,out] packet Pointer to allocated memory where extracted packet
 *                should be stored.
 *
 * @return Size of extracted packet (in bytes)
 * @return 0 if no packet was extracted.
 */
int net_client_dequeue_packet(NetClient* client, NetSPacket* packet);

/**
 * @brief Builds a protocol packet with payload equals to \p buf and places it
 *        to send pqueue.
 *
 * All packets placed into send pqueue will be sent to server during the next
 * call of net_client_tick function.
 *
 * @param[in] client   Pointer to net client instance.
 * @param[in] buf      Pointer to packet payload.
 * @param[in] size     Number of bytes in \p buf to be copied to packet payload.
 * @param[in] priority Priority of packet.
 */
void net_client_send(NetClient* client, const void* buf, int size,
                     NetPriority priority);

/**
 * @brief Returns current client state.
 *
 * @param[in] client Pointer to net client instance.
 *
 * @return NetClientState Current client state.
 */
NetClientState net_client_get_state(NetClient* client);

/**
 * @brief Returns a pointer to read-only server info structure.
 *
 * @param[in] client Pointer to net client instance.
 *
 * @return const NetServerInfo* Pointer to read-only server info structure.
 * @return NULL if \p client is NULL.
 */
const NetServerInfo* net_client_get_server_info(NetClient* client);

/**
 * @brief Returns id of the client.
 *
 * @param[in] client Pointer to network client instance.
 *
 * @return NetClientId Client id.
 * @return NET_CLIENT_ID_INVALID if \p client is NULL or not connected.
 */
NetClientId net_client_get_id(NetClient* client);

#endif /* NET_CLIENT */
