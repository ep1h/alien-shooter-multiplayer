/**
 * @file hook.h
 * @brief Component for setting/removing function hooks.
 * 
 */
#ifndef UTILS_HOOK_HOOK_H
#define UTILS_HOOK_HOOK_H

#include <stdbool.h>

/**
 * @brief Initializes the component.
 * 
 *        Initialization consists of allocating memory and initializing the
 *        container(s) to store information about installed hooks.
 *
 * @return true - success
 * @return false - fail
 */
bool hook_init(void);

/**
 * @brief Sets a hook to a function. If successful, all subsequent calls to the
 *        function at \p ​​function_address will be redirected to \p hook_address
 *
 * @param[in] function_address Address of a function to be hooked.
 * @param[in] hook_address     Address where \p function_address function calls
 *                             should be redirected.
 * @param[in] size             Number of bytes to be overwritten by jmp at
 *                             \p hook_address . Minimum possible value is 5.
 * 
 * @return Address of a trampoline to original function.
 */
void* hook_set(void* function_address, void* hook_address, unsigned char size);

/**
 * @brief Removes hook from function at address \p function_address (if exists).
 *
 * @param[in] function_address Function address on which a hook was previously
 *                             set.
 */
void hook_unset(void* function_address);

/**
 * @brief Unset all hooks, frees all memory allocated by the component since
 *        initialization.
 */
void hook_destroy(void);

#endif /* UTILS_HOOK_HOOK_H */
