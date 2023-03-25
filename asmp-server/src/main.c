#include <windows.h>
#include "server.h"

#ifndef EZTEST

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    MpServer* server = mp_server_create(8888, 4);
    if (server)
    {
        while (1)
        {
            mp_server_tick(server);
        }
        mp_server_destroy(server);
    }
    return 0;
}

#endif /* EZTEST */
