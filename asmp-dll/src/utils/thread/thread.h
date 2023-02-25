#ifndef UTILS_THREAD_THREAD_H
#define UTILS_THREAD_THREAD_H

typedef struct Thread Thread;

Thread* thread_create(void* function);
Thread* thread_create_with_args(void* function, void* args);
void thread_destroy(Thread* thread, int exit_code);

void thread_exit_current(int exit_code);

#endif /* UTILS_THREAD_THREAD_H */
