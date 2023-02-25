#include "../eztest/eztest.h"
#include "utils/containers/map/map.h"
#include "utils/mem/mem.h"

static int callback_calls_number;

static void map_foreach_callback(int key, void* data)
{
    *(int*)data *= 1000;
    *(int*)data += key;
    ++callback_calls_number;
}

TEST_BEGIN(test_1)
{
    int allocs = mem_debug_get_diff();

    Map* m = map_create();
    EXPECT_NOT_ZERO(m);
    EXPECT_NOT_ZERO(map_insert(m, 1, (void*)0xFFFFFFF1));
    EXPECT_NOT_ZERO(map_insert(m, 3, (void*)0xFFFFFFF3));
    EXPECT_NOT_ZERO(map_insert(m, 5, (void*)0xFFFFFFF5));
    EXPECT_NOT_ZERO(map_insert(m, 7, (void*)0xFFFFFFF7));
    EXPECT_ZERO(map_insert(m, 3, (void*)0xEEEEEEE3));
    EXPECT_ZERO(map_insert(0, 2, (void*)0xEEEEEEE2));
    EXPECT(map_find(m, 1), (void*)0xFFFFFFF1);
    EXPECT(map_find(m, 3), (void*)0xFFFFFFF3);
    EXPECT(map_find(m, 5), (void*)0xFFFFFFF5);
    EXPECT(map_find(m, 7), (void*)0xFFFFFFF7);
    EXPECT_ZERO(map_find(m, 4));
    EXPECT_NOT_ZERO(map_erase(m, 5));
    EXPECT_ZERO(map_find(m, 5));
    EXPECT_ZERO(map_erase(m, 6));
    map_clear(m);
    EXPECT_ZERO(map_find(m, 1));
    EXPECT_ZERO(map_find(m, 3));
    EXPECT_ZERO(map_find(m, 7));
    map_insert(m, 4, (void*)0xFFFFFFF4);
    EXPECT_NOT_ZERO(map_find(m, 4));
    EXPECT_NOT_ZERO(map_erase(m, 4));
    EXPECT_ZERO(map_find(m, 4));
    map_destroy(m);
    EXPECT(mem_debug_get_diff(), allocs);
    m = map_create();

    int v1 = 4;
    int v2 = 3;
    int v3 = 2;
    int v4 = 1;

    EXPECT_NOT_ZERO(map_insert(m, 1, (void*)&v1));
    EXPECT_NOT_ZERO(map_insert(m, 2, (void*)&v2));
    EXPECT_NOT_ZERO(map_insert(m, 3, (void*)&v3));
    EXPECT_NOT_ZERO(map_insert(m, 4, (void*)&v4));
    map_foreach(m, map_foreach_callback);
    EXPECT(v1, 4001);
    EXPECT(v2, 3002);
    EXPECT(v3, 2003);
    EXPECT(v4, 1004);
    EXPECT(callback_calls_number, 4);
    map_destroy(m);
    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(FILENAME, test_1);
