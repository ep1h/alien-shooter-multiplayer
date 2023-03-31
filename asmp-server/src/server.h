/**
 * @file server.h
 * @brief Top level server-side multiplayer logic.
 * 
*/
#ifndef MULTIPLAYER_SERVER_H
#define MULTIPLAYER_SERVER_H

typedef struct MpServer MpServer;

/**
 * @brief Creates top-level server instance.
 *
 * @param port        Server port.
 * @param max_clients Maximum number of clients.
 *
 * @return MpServer* Pointer to created server instance.
 */
MpServer* mp_server_create(unsigned short port, int max_clients);

/**
 * @brief Destroys top-level server instance.
 *
 * @param server Pointer to server instance.
 */
void mp_server_destroy(MpServer* server);

/**
 * @brief Processes server's internal state. Switches between states.
 *
 * @param server A pointer to server instance.
 */
void mp_server_tick(MpServer* server);

#endif /* MULTIPLAYER_SERVER_H */
