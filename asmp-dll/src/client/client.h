#ifndef MULTIPLAYER_CLIENT_H
#define MULTIPLAYER_CLIENT_H

#include <stdbool.h>

typedef struct MpClient MpClient;

typedef enum MpClientState
{
    MPS_UNINITED = 0,
    MPS_INITED,
    MPS_CONNECTING,
    MPS_CONNECTED,
    MPS_CONNECTION_FAILED,
    MPS_DISCONNECTION,
    MPS_DISCONNECTED,
} MpClientState;

MpClient* mp_client_create(void);
void mp_client_destroy(MpClient* client);
void mp_client_connection_request(MpClient* client, const char* ip,
                                  unsigned short port, const char* name);
void mp_client_disconnect(MpClient* client);

void mp_client_tick(MpClient* client);

MpClientState mp_client_get_state(MpClient* client);

#endif /* MULTIPLAYER_CLIENT_H */
