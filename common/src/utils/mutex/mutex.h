#ifndef UTILS_MUTEX_MUTEX_H
#define UTILS_MUTEX_MUTEX_H

typedef struct Mutex Mutex;

Mutex* mutex_create(void);
void mutex_destroy(Mutex* mutex);

void mutex_lock(Mutex* mutex);
void mutex_unlock(Mutex* mutex);

#endif /* UTILS_MUTEX_MUTEX_H */
