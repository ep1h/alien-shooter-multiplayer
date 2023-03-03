#include "pqueue.h"
#include <stdlib.h>
#include "utils/mem/mem.h"
#ifdef PQUEUE_MULTITHREAD_SAFE
#include "utils/mutex/mutex.h"
#endif /* PQUEUE_MULTITHREAD_SAFE */

typedef struct PQueueItem PQueueItem;

typedef enum PQueueType
{
    PQUEUE_TYPE_FIFO,
    PQUEUE_TYPE_LIFO,
} PQueueType;

typedef struct PQueue
{
    PQueueItem* items;
    int items_number;
    int capacity;
    PQueueType type;
#ifdef PQUEUE_MULTITHREAD_SAFE
    Mutex* mutex;
#endif /* PQUEUE_MULTITHREAD_SAFE */
} PQueue;

typedef struct PQueueItem
{
    int priority;
    void* data;
} PQueueItem;

// static int srot_func(const void *a, const void *b)
static void _queue_sort_after_insertation(PQueue* q);
static PQueue* _pqueue_create(void);

// static int srot_func(const void *a, const void *b)
// {
//     if (((const QueueItem *)a)->priority == ((const QueueItem *)b)->priority)
//     {
//         return a - b;
//     }
//     return (((const QueueItem *)a)->priority -
//             ((const QueueItem *)b)->priority);
// }

static void _queue_sort_after_insertation(PQueue* q)
{
    if (!q || q->items_number < 2)
    {
        return;
    }
    PQueueItem* a = &q->items[q->items_number - 1];
    PQueueItem tmp = {.data = a->data, .priority = a->priority};
    if (q->type == PQUEUE_TYPE_FIFO)
    {
        for (int i = q->items_number - 2; i >= 0; i--)
        {
            PQueueItem* b = &q->items[i];
            // TODO: Search for a place for new item, use memcpy and break.
            if (b->priority >= a->priority) // TODO: Change >= to > for stack.
            {
                /* Swap */
                a->priority = b->priority;
                a->data = b->data;
                b->priority = tmp.priority;
                b->data = tmp.data;
                a = b;
            }
        }
    }
    else // if (q->type == PQUEUE_TYPE_LIFO)
    {
        for (int i = q->items_number - 2; i >= 0; i--)
        {
            PQueueItem* b = &q->items[i];
            // TODO: Search for a place for new item, use memcpy and break.
            if (b->priority > a->priority)
            {
                /* Swap */
                a->priority = b->priority;
                a->data = b->data;
                b->priority = tmp.priority;
                b->data = tmp.data;
                a = b;
            }
        }
    }
}

static PQueue* _pqueue_create(void)
{
    PQueue* q = mem_alloc(sizeof(PQueue));
    if (!q)
    {
        return 0;
    }
    mem_set(q, 0, sizeof(PQueue));
#ifdef PQUEUE_MULTITHREAD_SAFE
    q->mutex = mutex_create();
    if (!q->mutex)
    {
        mem_free(q);
        return 0;
    }
#endif /* PQUEUE_MULTITHREAD_SAFE */
    return q;
}

PQueue* pqueue_create_fifo(void)
{
    PQueue* q = _pqueue_create();
    if (!q)
    {
        return 0;
    }
    q->type = PQUEUE_TYPE_FIFO;
    return q;
}

PQueue* pqueue_create_lifo(void)
{
    PQueue* q = _pqueue_create();
    if (!q)
    {
        return 0;
    }
    q->type = PQUEUE_TYPE_LIFO;
    return q;
}

void pqueue_destroy(PQueue* pqueue)
{
    if (!pqueue)
    {
        return;
    }
    if (pqueue->items)
    {
        mem_free(pqueue->items);
    }
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_destroy(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
    mem_free(pqueue);
}

bool pqueue_queue(PQueue* pqueue, const void* data, int priority)
{
    if (!pqueue)
    {
        return false;
    }
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_lock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
    /* Realloc if required */
    if (pqueue->items_number >= pqueue->capacity)
    {
        int new_capacity = 0;
        if (pqueue->capacity != 0)
        {
            new_capacity = pqueue->capacity * 2;
        }
        else
        {
            new_capacity = 1;
        }
        void* new_data =
            mem_realloc(pqueue->items, new_capacity * sizeof(PQueueItem));
        if (!new_data)
        {
#ifdef PQUEUE_MULTITHREAD_SAFE
            mutex_unlock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
            return false;
        }
        pqueue->items = new_data;
        pqueue->capacity = new_capacity;
    }
    pqueue->items[pqueue->items_number].priority = priority;
    pqueue->items[pqueue->items_number].data = (void*)data;
    ++pqueue->items_number;
    // TODO: Use quick sort!
    // qsort(queue->data, queue->items_number, sizeof(QueueItem), srot_func);
    // bsort(queue);
    _queue_sort_after_insertation(pqueue);
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_unlock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
    return true;
}

void* pqueue_dequeue(PQueue* pqueue)
{
    if (!pqueue)
    {
        return 0;
    }
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_lock(pqueue->mutex);
#endif /* OQUEUE_MULTITHREAD_SAFE */
    if (pqueue->items_number == 0)
    {
#ifdef PQUEUE_MULTITHREAD_SAFE
        mutex_unlock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
        return 0;
    }
    void* result = pqueue->items[pqueue->items_number - 1].data;
    --pqueue->items_number;
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_unlock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
    return result;
}

void pqueue_clear(PQueue* pqueue)
{
    if (!pqueue)
    {
        return;
    }
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_lock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
    pqueue->items_number = 0;
    pqueue->capacity = 0;
    mem_free(pqueue->items);
    pqueue->items = 0;
#ifdef PQUEUE_MULTITHREAD_SAFE
    mutex_unlock(pqueue->mutex);
#endif /* PQUEUE_MULTITHREAD_SAFE */
}

int pqueue_get_items_number(PQueue* pqueue)
{
    if (!pqueue)
    {
        return 0;
    }
    return pqueue->items_number;
}
