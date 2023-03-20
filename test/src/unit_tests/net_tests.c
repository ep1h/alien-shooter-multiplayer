// #include "../eztest/eztest.h"
// #include "net/server/server.h"
// #include "net/client/client.h"
// #include "utils/mem/mem.h"
// #include <winsock2.h>
// #include "utils/event/event.h"

// static Event* ev_connect_ = 0;
// static Event* ev_disconnect_ = 0;
// static Event* ev_recv_ = 0;

// static int on_connect_id_ = 0xCC;
// static int on_disconnect_id_ = 0xCC;
// static char buf_[1024] = {0};
// static NetPacket* packet_ = (NetPacket*)buf_;
// static int recv_size_ = 0xCC;

// void on_connect_(NetClientId id)
// {
//     on_connect_id_ = id;
//     event_trigger(ev_connect_);
// }

// void on_disconnect_(NetClientId id)
// {
//     on_disconnect_id_ = id;
//     event_trigger(ev_disconnect_);
// }

// void on_recv_(NetPacket* packet, int size)
// {
//     //event_wait_timeout(ev_recv_, 2000);
//     mem_copy(packet_, packet, size);
//     event_trigger(ev_recv_);
//     recv_size_ = size;
// }

// TEST_BEGIN(net_tests_main)
// {
//     ev_connect_ = event_create();
//     ev_disconnect_ = event_create();
//     ev_recv_ = event_create();

//     // int allocs = mem_debug_get_diff();

//     unsigned short port = 27015;
//     NetClientId max_clients = 4;
//     NetTime recv_timeout_ms = 5000;
//     NetTime send_timeout_ms = 5000;

//     NetServer* s =
//         net_server_create(port, max_clients, recv_timeout_ms, send_timeout_ms);
//     net_server_set_connect_callback(s, on_connect_);
//     net_server_set_disconnect_callback(s, on_disconnect_);
//     net_server_set_recv_callback(s, on_recv_);

//     EXPECT_NOT_ZERO(s);

//     NetClient* c = net_client_create();
//     EXPECT_NOT_ZERO(c);

//     EXPECT(net_server_is_client_connected(s, 0), false);
//     EXPECT(net_server_is_client_connected(s, 1), false);
//     EXPECT(net_server_is_client_connected(s, 2), false);
//     EXPECT(net_server_is_client_connected(s, 3), false);

//     EXPECT(net_client_connection_request(c, "127.0.0.1", port), true);

//     while ((net_client_get_state(c) != NCS_CONNECTED) &&
//            net_client_get_state(c) != NCS_CONNECTION_FAILED)
//     {
//         net_client_tick(c);
//     }
//     EXPECT(net_client_get_state(c), NCS_CONNECTED);

//     event_wait_timeout(ev_connect_, 500);
//     EXPECT(on_connect_id_, 0);

//     EXPECT(net_server_is_client_connected(s, 0), true);
//     EXPECT(net_server_is_client_connected(s, 1), false);
//     EXPECT(net_server_is_client_connected(s, 2), false);
//     EXPECT(net_server_is_client_connected(s, 3), false);

//     Sleep(100);
//     //unsigned char buf[512] = {0};
//     //NetPacket* p = (NetPacket*)buf;


//     net_client_send(c, "c:hello", 8, 0);
//     //event_wait_timeout(ev_recv_, 5000);
//     Sleep(100);
//     EXPECT(recv_size_, sizeof(NetPacket) + 8);
//     net_client_send(c, "c:hello_", 9, 0);
//     //event_wait_timeout(ev_recv_, 5000);
//     EXPECT(recv_size_, sizeof(NetPacket) + 9);
//     net_client_send(c, "c:hello_)", 10, 0);
//     //event_wait_timeout(ev_recv_, 5000);
//     EXPECT(recv_size_, sizeof(NetPacket) + 10);
//     net_client_tick(c);

//     // EXPECT()
//     // EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacket) + 8);
//     // EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacket) + 9);
//     // EXPECT(net_server_dequeue_packet(s, p), sizeof(NetPacket) + 10);
//     // EXPECT(net_server_dequeue_packet(s, p), 0);





//     // net_client_disconnect(c);
//     // Sleep(100);
//     // EXPECT(net_server_is_client_connected(s, 0), false);
//     // EXPECT(net_server_is_client_connected(s, 1), false);
//     // EXPECT(net_server_is_client_connected(s, 2), false);
//     // EXPECT(net_server_is_client_connected(s, 3), false);

//     // net_client_destroy(c);
//     // net_server_destroy(s);

//     // EXPECT(mem_debug_get_diff(), allocs);
// }
// TEST_END

// RUN_TESTS(FILENAME, net_tests_main);
