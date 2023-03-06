#include <windows.h>
#include "server.h"

#include "net/server/server.h"
#ifndef EZTEST

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    NetServer* s = net_server_create(8888, 4, 5000, 5000);
    while (s)
    {
        Sleep(10000);
    }
    net_server_destroy(s);
    return 0;
}

#endif /* EZTEST */
