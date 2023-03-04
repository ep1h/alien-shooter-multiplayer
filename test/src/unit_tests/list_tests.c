#include "../eztest/eztest.h"
#include "utils/containers/list/list.h"
#include "utils/mem/mem.h"

TEST_BEGIN(test_1)
{
    int allocs = mem_debug_get_diff();

    EXPECT_ZERO(list_push_back(0, (void*)123));
    EXPECT_ZERO(list_push_front(0, (void*)123));
    EXPECT_ZERO(list_pop_back(0));
    EXPECT_ZERO(list_pop_front(0));
    EXPECT_ZERO(list_get_items_number(0));

    List* l = list_create();
    EXPECT_ZERO(list_get_items_number(l));
    EXPECT_NOT_ZERO(list_push_back(l, (void*)5));
    EXPECT_NOT_ZERO(list_push_back(l, (void*)6));
    EXPECT_NOT_ZERO(list_push_back(l, (void*)7));
    EXPECT_NOT_ZERO(list_push_back(l, (void*)8));
    EXPECT_NOT_ZERO(list_push_front(l, (void*)4));
    EXPECT_NOT_ZERO(list_push_front(l, (void*)3));
    EXPECT_NOT_ZERO(list_push_front(l, (void*)2));
    EXPECT_NOT_ZERO(list_push_front(l, (void*)1));
    EXPECT(list_get_items_number(l), 8);
    EXPECT(list_pop_back(l), 8);
    EXPECT(list_pop_back(l), 7);
    EXPECT(list_pop_front(l), 1);
    EXPECT(list_pop_front(l), 2);
    EXPECT(list_get_items_number(l), 4);
    EXPECT(list_pop_back(l), 6);
    EXPECT(list_pop_back(l), 5);
    EXPECT(list_get_items_number(l), 2);
    EXPECT(list_pop_back(l), 4);
    EXPECT(list_get_items_number(l), 1);
    EXPECT(list_pop_front(l), 3);
    EXPECT(list_get_items_number(l), 0);
    EXPECT(list_pop_front(l), 0);
    EXPECT(list_get_items_number(l), 0);
    EXPECT_NOT_ZERO(list_push_front(l, (void*)11));
    EXPECT(list_pop_back(l), 11);
    EXPECT_NOT_ZERO(list_push_back(l, (void*)10));
    EXPECT(list_pop_front(l), 10);
    EXPECT_NOT_ZERO(list_push_front(l, (void*)555));
    EXPECT_NOT_ZERO(list_push_front(l, (void*)555));
    EXPECT_NOT_ZERO(list_push_front(l, (void*)555));
    EXPECT(list_get_items_number(l), 3);
    list_clear(l);
    EXPECT(list_get_items_number(l), 0);
    EXPECT_ZERO(list_pop_back(l));
    EXPECT_ZERO(list_pop_front(l));
    list_destroy(l);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(FILENAME, test_1);
