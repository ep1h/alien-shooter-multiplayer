#include "../eztest/eztest.h"
#include "utils/containers/pqueue/pqueue.h"
#include "utils/mem/mem.h"

TEST_BEGIN(pqueue_fifo_test)
{
    int allocs = mem_debug_get_diff();

    EXPECT_ZERO(pqueue_queue(0, (const void*)0xF, 1));
    EXPECT_ZERO(pqueue_dequeue(0));
    EXPECT_ZERO(pqueue_get_items_number(0));

    PQueue* q = pqueue_create_fifo();
    EXPECT_NOT_ZERO(q);
    EXPECT_ZERO(pqueue_dequeue(q));
    EXPECT_ZERO(pqueue_get_items_number(q));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xA, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xB, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xC, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xAA, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xBB, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xCC, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xAAA, 3));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xBBB, 3));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xCCC, 3));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xD, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xDD, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xDDD, 3));
    EXPECT(pqueue_dequeue(q), 0xAAA);
    EXPECT(pqueue_dequeue(q), 0xBBB);
    EXPECT(pqueue_dequeue(q), 0xCCC);
    EXPECT(pqueue_dequeue(q), 0xDDD);
    EXPECT(pqueue_dequeue(q), 0xAA);
    EXPECT(pqueue_dequeue(q), 0xBB);
    EXPECT(pqueue_dequeue(q), 0xCC);
    EXPECT(pqueue_dequeue(q), 0xDD);
    EXPECT(pqueue_dequeue(q), 0xA);
    EXPECT(pqueue_dequeue(q), 0xB);
    EXPECT(pqueue_dequeue(q), 0xC);
    EXPECT(pqueue_dequeue(q), 0xD);
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x555, 5));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x666, 6));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x777, 7));
    pqueue_clear(q);
    EXPECT_ZERO(pqueue_get_items_number(q));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x1, 100));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x2, 20));
    EXPECT(pqueue_get_items_number(q), 2);
    EXPECT(pqueue_dequeue(q), 0x1);
    EXPECT(pqueue_get_items_number(q), 1);
    EXPECT(pqueue_dequeue(q), 0x2);
    EXPECT(pqueue_get_items_number(q), 0);
    pqueue_clear(q);
    EXPECT(pqueue_get_items_number(q), 0);
    pqueue_destroy(q);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

TEST_BEGIN(pqueue_lifo_test)
{
    int allocs = mem_debug_get_diff();

    EXPECT_ZERO(pqueue_queue(0, (const void*)0xF, 1));
    EXPECT_ZERO(pqueue_dequeue(0));
    EXPECT_ZERO(pqueue_get_items_number(0));

    PQueue* q = pqueue_create_lifo();
    EXPECT_NOT_ZERO(q);
    EXPECT_ZERO(pqueue_dequeue(q));
    EXPECT_ZERO(pqueue_get_items_number(q));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xA, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xB, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xC, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xAA, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xBB, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xCC, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xAAA, 3));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xBBB, 3));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xCCC, 3));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xD, 1));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xDD, 2));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0xDDD, 3));
    EXPECT(pqueue_dequeue(q), 0xDDD);
    EXPECT(pqueue_dequeue(q), 0xCCC);
    EXPECT(pqueue_dequeue(q), 0xBBB);
    EXPECT(pqueue_dequeue(q), 0xAAA);
    EXPECT(pqueue_dequeue(q), 0xDD);
    EXPECT(pqueue_dequeue(q), 0xCC);
    EXPECT(pqueue_dequeue(q), 0xBB);
    EXPECT(pqueue_dequeue(q), 0xAA);
    EXPECT(pqueue_dequeue(q), 0xD);
    EXPECT(pqueue_dequeue(q), 0xC);
    EXPECT(pqueue_dequeue(q), 0xB);
    EXPECT(pqueue_dequeue(q), 0xA);
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x555, 5));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x666, 6));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x777, 7));
    pqueue_clear(q);
    EXPECT_ZERO(pqueue_get_items_number(q));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x1, 100));
    EXPECT_NOT_ZERO(pqueue_queue(q, (const void*)0x2, 20));
    EXPECT(pqueue_get_items_number(q), 2);
    EXPECT(pqueue_dequeue(q), 0x1);
    EXPECT(pqueue_get_items_number(q), 1);
    EXPECT(pqueue_dequeue(q), 0x2);
    EXPECT(pqueue_get_items_number(q), 0);
    pqueue_clear(q);
    EXPECT(pqueue_get_items_number(q), 0);
    pqueue_destroy(q);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(FILENAME, pqueue_fifo_test, pqueue_lifo_test);
