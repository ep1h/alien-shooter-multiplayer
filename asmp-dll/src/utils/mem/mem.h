#ifndef UTILS_MEM_MEM_H
#define UTILS_MEM_MEM_H

void* mem_alloc(int size);
void* mem_realloc(void* address, unsigned int new_size);
void mem_free(void* address);
void mem_copy(void* dst, const void* src, unsigned int size);
void mem_set(void* dst, int value, unsigned int size);

#if defined EZTEST || defined DEBUG_
int mem_debug_get_diff(void);
#endif /* EZTEST || DEBUG_ */

#endif /* UTILS_MEM_MEM_H */
