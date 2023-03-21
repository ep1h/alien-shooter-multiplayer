#ifndef MULTIPLAYER_SERVER_H
#define MULTIPLAYER_SERVER_H

#include <stdbool.h>

typedef struct MpServer MpServer;

bool mp_server_init(unsigned short port, int max_clients);
void mp_server_destroy(void);

#endif /* MULTIPLAYER_SERVER_H */
