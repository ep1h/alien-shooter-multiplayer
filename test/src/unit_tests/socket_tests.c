#include "../eztest/eztest.h"
#include "net/socket/socket.h"
#include "utils/mem/mem.h"
#include <winsock2.h>

TEST_BEGIN(socket_tests_main)
{
    int allocs = mem_debug_get_diff();

    unsigned short port = 27015;

    /* Init winsock lib */
    EXPECT(net_socket_init(), true);

    /* Create and configure server socket */
    NetSocket* ss = net_socket_create_socket();
    net_socket_set_recvfrom_timeout(ss, 2000);
    EXPECT_NOT_ZERO(ss);
    EXPECT_NOT_ZERO(net_socket_bind_socket(ss, 0, port));

    /* Create and configure client socket */
    NetSocket* cs = net_socket_create_socket();
    EXPECT_NOT_ZERO(cs);
    net_socket_set_recvfrom_timeout(cs, 2000);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27015);
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    /* Send client -> server */
    EXPECT(net_socket_sendto(cs, &server_addr, "c->s 1", 7), 7);

    /* Recv it on server side */
    struct sockaddr_in sender = {0};
    char buf[512] = {0};
    EXPECT(net_socket_recvfrom(ss, &sender, buf), 7);
    EXPECT_BUF(buf, "c->s 1", 6);
    mem_set(buf, 0, sizeof(buf));

    /* Send server -> client */
    EXPECT(net_socket_sendto(ss, &sender, "s->c 22", 8), 8);

    /* Recv it on client side */
    EXPECT(net_socket_recvfrom(cs, &sender, buf), 8);
    EXPECT_BUF(buf, "s->c 22", 8);
    mem_set(buf, 0, sizeof(buf));

    /* Send client -> server */
    EXPECT(net_socket_sendto(cs, &sender, "c->s 333", 9), 9);

    /* Recv it on server side */
    EXPECT(net_socket_recvfrom(ss, &sender, buf), 9);
    EXPECT_BUF(buf, "c->s 333", 9);
    mem_set(buf, 0, sizeof(buf));

    /* Don't send anything else */
    EXPECT(net_socket_recvfrom(cs, &sender, buf), NET_SOCKET_ERROR_TIMEDOUT);
    EXPECT(net_socket_recvfrom(ss, &sender, buf), NET_SOCKET_ERROR_TIMEDOUT);

    /* Cleanup */
    net_socket_destroy_socket(cs);
    net_socket_destroy_socket(ss);
    net_socket_destroy();

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(socket_tests, socket_tests_main);
