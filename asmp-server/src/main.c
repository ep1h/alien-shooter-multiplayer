#include <windows.h>
#include "server.h"

#ifndef EZTEST


int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    if (!mp_server_init(8888, 4))
    {
        return -1;
    }

    while (1)
    {
        Sleep(10000);
    }

    mp_server_destroy();
    return 0;
}

#endif /* EZTEST */
