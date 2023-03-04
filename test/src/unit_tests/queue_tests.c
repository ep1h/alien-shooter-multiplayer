#include "../eztest/eztest.h"
#include "utils/containers/queue/queue.h"
#include "utils/mem/mem.h"

TEST_BEGIN(queue_fifo_test)
{
    int allocs = mem_debug_get_diff();

    EXPECT_ZERO(queue_queue(0, (const void*)0xF));
    EXPECT_ZERO(queue_dequeue(0));
    EXPECT_ZERO(queue_get_items_number(0));

    Queue* q = queue_create_fifo();
    EXPECT_NOT_ZERO(q);
    EXPECT_ZERO(queue_dequeue(q));
    EXPECT_ZERO(queue_get_items_number(q));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xA));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xB));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xC));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xD));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xAA));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xBB));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xCC));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xDD));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xAAA));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xBBB));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xCCC));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xDDD));
    EXPECT(queue_dequeue(q), 0xA);
    EXPECT(queue_dequeue(q), 0xB);
    EXPECT(queue_dequeue(q), 0xC);
    EXPECT(queue_dequeue(q), 0xD);
    EXPECT(queue_dequeue(q), 0xAA);
    EXPECT(queue_dequeue(q), 0xBB);
    EXPECT(queue_dequeue(q), 0xCC);
    EXPECT(queue_dequeue(q), 0xDD);
    EXPECT(queue_dequeue(q), 0xAAA);
    EXPECT(queue_dequeue(q), 0xBBB);
    EXPECT(queue_dequeue(q), 0xCCC);
    EXPECT(queue_dequeue(q), 0xDDD);
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x555));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x666));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x777));
    queue_clear(q);
    EXPECT_ZERO(queue_get_items_number(q));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x1));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x2));
    EXPECT(queue_get_items_number(q), 2);
    EXPECT(queue_dequeue(q), 0x1);
    EXPECT(queue_get_items_number(q), 1);
    EXPECT(queue_dequeue(q), 0x2);
    EXPECT(queue_get_items_number(q), 0);
    queue_clear(q);
    EXPECT(queue_get_items_number(q), 0);
    queue_destroy(q);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

TEST_BEGIN(queue_lifo_test)
{
    int allocs = mem_debug_get_diff();

    EXPECT_ZERO(queue_queue(0, (const void*)0xF));
    EXPECT_ZERO(queue_dequeue(0));
    EXPECT_ZERO(queue_get_items_number(0));

    Queue* q = queue_create_lifo();
    EXPECT_NOT_ZERO(q);
    EXPECT_ZERO(queue_dequeue(q));
    EXPECT_ZERO(queue_get_items_number(q));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xA));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xB));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xC));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xD));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xAA));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xBB));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xCC));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xDD));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xAAA));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xBBB));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xCCC));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0xDDD));
    EXPECT(queue_dequeue(q), 0xDDD);
    EXPECT(queue_dequeue(q), 0xCCC);
    EXPECT(queue_dequeue(q), 0xBBB);
    EXPECT(queue_dequeue(q), 0xAAA);
    EXPECT(queue_dequeue(q), 0xDD);
    EXPECT(queue_dequeue(q), 0xCC);
    EXPECT(queue_dequeue(q), 0xBB);
    EXPECT(queue_dequeue(q), 0xAA);
    EXPECT(queue_dequeue(q), 0xD);
    EXPECT(queue_dequeue(q), 0xC);
    EXPECT(queue_dequeue(q), 0xB);
    EXPECT(queue_dequeue(q), 0xA);
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x555));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x666));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x777));
    queue_clear(q);
    EXPECT_ZERO(queue_get_items_number(q));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x1));
    EXPECT_NOT_ZERO(queue_queue(q, (const void*)0x2));
    EXPECT(queue_get_items_number(q), 2);
    EXPECT(queue_dequeue(q), 0x2);
    EXPECT(queue_get_items_number(q), 1);
    EXPECT(queue_dequeue(q), 0x1);
    EXPECT(queue_get_items_number(q), 0);
    queue_clear(q);
    EXPECT(queue_get_items_number(q), 0);
    queue_destroy(q);

    EXPECT(mem_debug_get_diff(), allocs);
}
TEST_END

RUN_TESTS(FILENAME, queue_fifo_test, queue_lifo_test);
