#ifndef UTILS_CONTAINERS_QUEUE_QUEUE_H
#define UTILS_CONTAINERS_QUEUE_QUEUE_H

#include <stdbool.h>

#define QUEUE_MULTITHREAD_SAFE

typedef struct Queue Queue;

Queue* queue_create_fifo(void);
Queue* queue_create_lifo(void);

void queue_destroy(Queue* queue);

bool queue_queue(Queue* queue, const void* data);
void* queue_dequeue(Queue* queue);

void queue_clear(Queue* queue);
int queue_get_items_number(Queue* queue);

#endif /* UTILS_CONTAINERS_QUEUE_QUEUE_H */
