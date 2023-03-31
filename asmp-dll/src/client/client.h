/**
 * @file client.h
 * @brief Top level client-side multiplayer logic.
 *
 */
#ifndef MULTIPLAYER_CLIENT_H
#define MULTIPLAYER_CLIENT_H

#include <stdbool.h>
#include "multiplayer_protocol.h"

typedef struct MpClient MpClient;

/**
 * @brief Client state.
 */
typedef enum MpClientState
{
    MP_CLIENT_STATE_UNINITED = 0,
    MP_CLIENT_STATE_INITED,
    MP_CLIENT_STATE_CONNECTING,
    MP_CLIENT_STATE_CONNECTED,
    MP_CLIENT_STATE_CONNECTION_FAILED,
    MP_CLIENT_STATE_DISCONNECTING,
} MpClientState;

/**
 * @brief Creates top-level client instance.
 *
 * @return MpClient* Pointer to created client instance.
 */
MpClient* mp_client_create(void);

/**
 * @brief Destroys top-level client instance.
 *
 * @param client Pointer to client instance.
 */
void mp_client_destroy(MpClient* client);

/**
 * @brief Processes client's internal state. Switches between states.
 *
 * @param client A pointer to client instance.
 */
void mp_client_tick(MpClient* client);

/**
 * @brief Sends connection request to server.
 *
 * @param client Pointer to client instance.
 * @param ip     Server IP address.
 * @param port   Server port.
 * @param name   Player name. Max length is defined in MP_MAX_NAME_LEN macro.
 *
 * @return true if the client has not yet been connected, current client state
 *         allows connection request and arguments are valid.
 */
bool mp_client_connection_request(MpClient* client, const char* ip,
                                  uint16_t port, const char* name);

/**
 * @brief Disconnects from server. Clear all data related to current session.
 *
 * @param client Pointer to client instance.
 */
void mp_client_disconnect(MpClient* client);

/**
 * @brief Sets callback that will be called when actor info of an actor is
 *        received from server.
 *
 * @param client   Pointer to client instance.
 * @param callback Callback function pointer.
 */
void mp_client_set_actor_sync_callback(MpClient* client,
                                       void (*callback)(MpClient* client,
                                                        int id,
                                                        MpActor* actor));

/**
 * @brief Returns current client state.
 *
 * @param client Pointer to client instance.
 *
 * @return MpClientState Current client state.
 */
MpClientState mp_client_get_state(MpClient* client);

/**
 * @brief Returns configuration of the server that client is connected to.
 *
 * @param client Pointer to client instance.
 *
 * @return const MpServerConfiguration* Read-only server configuration pointer.
 */
const MpServerConfiguration* mp_client_get_server_configuration(
    MpClient* client);

/**
 * @brief Returns maximum number of players that can be connected to the server.
 *
 * @param client Pointer to client instance.
 *
 * @return int Maximum number of players.
 */
int mp_client_get_max_players_number(MpClient* client);

/**
 * @brief Returns pointer to client's struct that contains local player's data.
 *
 * @param client Pointer to client instance.
 *
 * @return MpPlayer* Pointer to local player's struct.
 */
MpPlayer* mp_client_get_local_player(MpClient* client);

// /**
//  * @brief Returns pointer to read-only remote player's struct.
//  *
//  * @param client Pointer to client instance.
//  * @param id     Remote player's id.
//  *
//  * @return const MpPlayer* Read-only remote player's struct pointer.
//  */
// const MpPlayer* mp_client_get_remote_player(MpClient* client, int id);

#endif /* MULTIPLAYER_CLIENT_H */
