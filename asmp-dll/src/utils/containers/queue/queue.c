#include "queue.h"
#include <stdlib.h>
#include "utils/mem/mem.h"
#ifdef QUEUE_MULTITHREAD_SAFE
#include "utils/mutex/mutex.h"
#endif /* QUEUE_MULTITHREAD_SAFE */

typedef struct QueueItem QueueItem;

typedef enum QueueType
{
    QUEUE_TYPE_FIFO,
    QUEUE_TYPE_LIFO,
} QueueType;

typedef struct Queue
{
    QueueItem* items;
    int items_number;
    int capacity;
    QueueType type;
#ifdef QUEUE_MULTITHREAD_SAFE
    Mutex* mutex;
#endif /* QUEUE_MULTITHREAD_SAFE */
} Queue;

typedef struct QueueItem
{
    void* data;
} QueueItem;

static Queue* _queue_create(void);

static Queue* _queue_create(void)
{
    Queue* q = mem_alloc(sizeof(Queue));
    if (!q)
    {
        return 0;
    }
    mem_set(q, 0, sizeof(Queue));
#ifdef QUEUE_MULTITHREAD_SAFE
    q->mutex = mutex_create();
    if (!q->mutex)
    {
        mem_free(q);
        return 0;
    }
#endif /* QUEUE_MULTITHREAD_SAFE */
    return q;
}

Queue* queue_create_fifo(void)
{
    Queue* q = _queue_create();
    if (!q)
    {
        return 0;
    }
    q->type = QUEUE_TYPE_FIFO;
    return q;
}

Queue* queue_create_lifo(void)
{
    Queue* q = _queue_create();
    if (!q)
    {
        return 0;
    }
    q->type = QUEUE_TYPE_LIFO;
    return q;
}

void queue_destroy(Queue* queue)
{
    if (!queue)
    {
        return;
    }
    if (queue->items)
    {
        mem_free(queue->items);
    }
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_destroy(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
    mem_free(queue);
}

bool queue_queue(Queue* queue, const void* data)
{
    if (!queue)
    {
        return false;
    }
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_lock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
    /* Realloc if required */
    if (queue->items_number >= queue->capacity)
    {
        int new_capacity = 0;
        if (queue->capacity != 0)
        {
            new_capacity = queue->capacity * 2;
        }
        else
        {
            new_capacity = 1;
        }
        void* new_data =
            mem_realloc(queue->items, new_capacity * sizeof(QueueItem));
        if (!new_data)
        {
#ifdef QUEUE_MULTITHREAD_SAFE
            mutex_unlock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
            return false;
        }
        queue->items = new_data;
        queue->capacity = new_capacity;
    }
    queue->items[queue->items_number].data = (void*)data;
    ++queue->items_number;
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_unlock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
    return true;
}

void* queue_dequeue(Queue* queue)
{
    if (!queue)
    {
        return 0;
    }
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_lock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
    if (queue->items_number == 0)
    {
#ifdef QUEUE_MULTITHREAD_SAFE
        mutex_unlock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
        return 0;
    }

    void* result = 0;
    if (queue->type == QUEUE_TYPE_FIFO) // TODO: Optimize dequeue for FIFO.
    {
        result = queue->items[0].data;
        mem_copy(&queue->items[0], &queue->items[1],
                 queue->items_number * sizeof(queue->items[0]));
    }
    else // if (queue->type == QUEUE_TYPE_LIFO)
    {
        result = queue->items[queue->items_number - 1].data;
    }

    --queue->items_number;
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_unlock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
    return result;
}

void queue_clear(Queue* queue)
{
    if (!queue)
    {
        return;
    }
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_lock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
    queue->items_number = 0;
    queue->capacity = 0;
    mem_free(queue->items);
    queue->items = 0;
#ifdef QUEUE_MULTITHREAD_SAFE
    mutex_unlock(queue->mutex);
#endif /* QUEUE_MULTITHREAD_SAFE */
}

int queue_get_items_number(Queue* queue)
{
    if (!queue)
    {
        return 0;
    }
    return queue->items_number;
}
