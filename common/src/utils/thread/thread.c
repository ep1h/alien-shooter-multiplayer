#include "thread.h"
#include <windows.h>
#include "utils//mem/mem.h"

typedef struct Thread
{
    DWORD id;
    HANDLE handle;
} Thread;

Thread* thread_create(void* function)
{
    Thread* t = mem_alloc(sizeof(Thread));
    if (!t)
    {
        return 0;
    }
    t->handle =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, 0, 0, &t->id);
    if (!t->handle)
    {
        mem_free(t);
        return 0;
    }
    return t;
}

Thread* thread_create_with_args(void* function, void* args)
{
    Thread* t = mem_alloc(sizeof(Thread));
    if (!t)
    {
        return 0;
    }
    t->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, args, 0,
                             &t->id);
    if (!t->handle)
    {
        mem_free(t);
        return 0;
    }
    return t;
}

void thread_destroy(Thread* thread, int exit_code)
{
    if (!thread)
    {
        return;
    }
    TerminateThread(thread->handle, exit_code);
    CloseHandle(thread->handle);
    mem_free(thread);
}

void thread_exit_current(int exit_code)
{
    ExitThread(exit_code);
}
