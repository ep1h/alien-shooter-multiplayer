#include "../eztest/eztest.h"
#include "multiplayer/net/server/server.h"
#include "multiplayer/net/client/client.h"
#include "utils/mem/mem.h"
#include <winsock2.h>

TEST_BEGIN(net_tests_main)
{
    int allocs = mem_debug_get_diff();

    unsigned short port = 27015;

    NetServer* s = net_server_create(port);
    EXPECT_NOT_ZERO(s);

    NetClient* c = net_client_create();
    EXPECT_NOT_ZERO(c);

    EXPECT(net_server_is_client_connected(s, 0), false);
    EXPECT(net_server_is_client_connected(s, 1), false);
    EXPECT(net_server_is_client_connected(s, 2), false);
    EXPECT(net_server_is_client_connected(s, 3), false);

    EXPECT(net_client_connect(c, "127.0.0.1", port), true);

    EXPECT(net_server_is_client_connected(s, 0), true);
    EXPECT(net_server_is_client_connected(s, 1), false);
    EXPECT(net_server_is_client_connected(s, 2), false);
    EXPECT(net_server_is_client_connected(s, 3), false);

    unsigned char buf[512] = {0};
    NetPacket* p = (NetPacket*)buf;
    EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacketVirtual));
    NetPacketVirtual* vp = (NetPacketVirtual*)p;
    EXPECT(vp->virtual_type, NVPT_CLIENT_CONNECTED);
    EXPECT((int)vp->head.sender, 0);

    EXPECT(net_server_is_client_connected(s, 0), true);

    net_client_send(c, "c:hello", 8, 0);
    net_client_send(c, "c:hello_", 9, 0);
    net_client_send(c, "c:hello_)", 10, 0);
    Sleep(100);
    EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacket) + 8);
    EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacket) + 9);
    EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacket) + 10);
    EXPECT(net_server_dequeue_packet(s, p), 0);

    net_client_disconnect(c);
    Sleep(100);
    EXPECT(net_server_is_client_connected(s, 0), false);
    EXPECT(net_server_is_client_connected(s, 1), false);
    EXPECT(net_server_is_client_connected(s, 2), false);
    EXPECT(net_server_is_client_connected(s, 3), false);

    net_client_destroy(c);
    net_server_destroy(s);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(FILENAME, net_tests_main);
