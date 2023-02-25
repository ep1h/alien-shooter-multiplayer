#include "mutex.h"
#include "utils/mem/mem.h"
#include <windows.h>

typedef struct Mutex
{
    CRITICAL_SECTION cs;
} Mutex;

Mutex* mutex_create(void)
{
    Mutex* m = mem_alloc(sizeof(Mutex));
    if (!m)
    {
        return 0;
    }
    InitializeCriticalSection(&m->cs);
    return m;
}

void mutex_destroy(Mutex* mutex)
{
    if (!mutex)
    {
        return;
    }
    DeleteCriticalSection(&mutex->cs);
    mem_free(mutex);
}
void mutex_lock(Mutex* mutex)
{
    if (!mutex)
    {
        return;
    }
    EnterCriticalSection(&mutex->cs);
}

void mutex_unlock(Mutex* mutex)
{
    if (!mutex)
    {
        return;
    }
    LeaveCriticalSection(&mutex->cs);
}
