#ifndef MULTIPLAYER_SERVER_H
#define MULTIPLAYER_SERVER_H

#include <stdbool.h>

typedef struct MpServer MpServer;

MpServer* mp_server_create(unsigned short port, int max_clients);
void mp_server_destroy(MpServer* server);
void mp_server_tick(MpServer* server);

#endif /* MULTIPLAYER_SERVER_H */
