#ifndef UTILS_HOOK_HOOK_H
#define UTILS_HOOK_HOOK_H

#include <stdbool.h>

bool hook_init(void);
void* hook_set(void* function_address, void* hook_address, unsigned char size);
void hook_unset(void* function_address);
void* hook_set_vmt(void** function_address_ptr, void* hook_address);
void hook_destroy(void);

#endif /* UTILS_HOOK_HOOK_H */
