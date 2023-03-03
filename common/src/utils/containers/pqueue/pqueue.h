#ifndef UTILS_CONTAINERS_PQUEUE_PQUEUE_H
#define UTILS_CONTAINERS_PQUEUE_PQUEUE_H

#include <stdbool.h>

#define PQUEUE_MULTITHREAD_SAFE

typedef struct PQueue PQueue;

PQueue* pqueue_create_fifo(void);
PQueue* pqueue_create_lifo(void);

void pqueue_destroy(PQueue* pqueue);

bool pqueue_queue(PQueue* pqueue, const void* data, int priority);
void* pqueue_dequeue(PQueue* pqueue);

void pqueue_clear(PQueue* pqueue);
int pqueue_get_items_number(PQueue* pqueue);

#endif /* UTILS_CONTAINERS_PQUEUE_PQUEUE_H */
