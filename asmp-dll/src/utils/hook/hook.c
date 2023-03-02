#include "hook.h"
#include "utils/mem/mem.h"
#include "utils/containers/map/map.h"
#include <windows.h>

#define MAX_HOOKS        32
#define ASM_X86_JMP      0xE9
#define ASM_X86_NOP      0x90
#define ASM_X86_JMP_SIZE 5
#define MIN_HOOK_SIZE    ASM_X86_JMP_SIZE

typedef struct TrampolineHook
{
    void* function_address;
    void* trampoline_address;
    unsigned long size;
} TrampolineHook;

typedef struct VmtHook
{
    void** function_address_ptr;
    void* orig_function_address; /* Required to unset hook */
} VmtHook;

static Map* trampoline_hooks_map_;
static Map* vmt_hooks_map_;

static void callback_unset_trampoline_hooks_(int key, void* data);
static void callback_unset_vmt_hooks_(int key, void* data);
static void* set_jmp_hook_(void* function_address, void* hook_address,
                           unsigned char size);
static void unset_hook_(void* function_address, void* trampoline_address,
                        unsigned char size);
static void* set_vmt_hook_(void** function_address_ptr, void* hook_address);
static void unset_vmt_hook_(void** function_address_ptr,
                            void* orig_funcion_address);

static void callback_unset_trampoline_hooks_(int key, void* data)
{
    (void)key;
    TrampolineHook* hook = (TrampolineHook*)data;
    unset_hook_(hook->function_address, hook->trampoline_address, hook->size);
    mem_free(hook);
}

static void callback_unset_vmt_hooks_(int key, void* data)
{
    (void)key;
    VmtHook* hook = (VmtHook*)data;
    unset_vmt_hook_(hook->function_address_ptr, hook->orig_function_address);
    mem_free(hook);
}

static void* set_jmp_hook_(void* function_address, void* hook_address,
                           unsigned char size)
{
    if (!function_address || !hook_address || size < MIN_HOOK_SIZE)
    {
        return 0;
    }
    unsigned char* trampoline_address = (unsigned char*)VirtualAlloc(
        NULL, size + ASM_X86_JMP_SIZE, MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);
    if (!trampoline_address)
    {
        return 0;
    }
    memcpy(trampoline_address, function_address, size);
    trampoline_address[size] = ASM_X86_JMP;
    *(unsigned int*)(trampoline_address + size + 1) =
        (unsigned char*)function_address + size - (trampoline_address + size) -
        ASM_X86_JMP_SIZE;
    DWORD cur_prot = 0;
    if (!VirtualProtect(function_address, size, PAGE_EXECUTE_READWRITE,
                        &cur_prot))
    {
        VirtualFree(trampoline_address, size + ASM_X86_JMP_SIZE, MEM_FREE);
        return 0;
    }
    memset(function_address, ASM_X86_NOP, size);
    *(unsigned char*)function_address = ASM_X86_JMP;
    *(unsigned int*)((unsigned char*)function_address + 1) =
        (unsigned char*)hook_address - (unsigned char*)function_address -
        ASM_X86_JMP_SIZE;
    DWORD tmp_prot = 0;
    VirtualProtect(function_address, size, cur_prot, &tmp_prot);
    return trampoline_address;
}

static void unset_hook_(void* function_address, void* trampoline_address,
                        unsigned char size)
{
    if (!trampoline_address)
        return;
    DWORD cur_prot = 0;
    if (!VirtualProtect(function_address, size, PAGE_EXECUTE_READWRITE,
                        &cur_prot))
    {
        return;
    }
    memcpy(function_address, trampoline_address, size);
    VirtualProtect(function_address, size, PAGE_EXECUTE_READWRITE, &cur_prot);
    VirtualFree(trampoline_address, size, MEM_FREE);
}

static void* set_vmt_hook_(void** function_address_ptr, void* hook_address)
{
    if (!function_address_ptr || !*function_address_ptr || !hook_address)
    {
        return 0;
    }
    DWORD cur_prot = 0;
    if (!VirtualProtect(function_address_ptr, 4, PAGE_EXECUTE_READWRITE,
                        &cur_prot))
    {
        return 0;
    }
    void* orig_function_address = *function_address_ptr;
    *function_address_ptr = hook_address;
    DWORD tmp_prot = 0;
    VirtualProtect(function_address_ptr, 4, cur_prot, &tmp_prot);
    return orig_function_address;
}

static void unset_vmt_hook_(void** function_address_ptr,
                            void* orig_funcion_address)
{
    if (!function_address_ptr || !*function_address_ptr ||
        !orig_funcion_address)
    {
        return;
    }
    DWORD cur_prot = 0;
    if (!VirtualProtect(function_address_ptr, 4, PAGE_EXECUTE_READWRITE,
                        &cur_prot))
    {
        return;
    }
    *function_address_ptr = orig_funcion_address;
    DWORD tmp_prot = 0;
    VirtualProtect(function_address_ptr, 4, cur_prot, &tmp_prot);
    return;
}

bool hook_init(void)
{
    trampoline_hooks_map_ = map_create();
    if (trampoline_hooks_map_)
    {
        vmt_hooks_map_ = map_create();
        if (vmt_hooks_map_)
        {
            return true;
        }
        map_destroy(trampoline_hooks_map_);
    }
    return false;
}

void* hook_set(void* function_address, void* hook_address, unsigned char size)
{
    TrampolineHook* hook = mem_alloc(sizeof(TrampolineHook));
    if (!hook)
    {
        return 0;
    }
    hook->trampoline_address =
        set_jmp_hook_(function_address, hook_address, size);
    if (!hook->trampoline_address)
    {
        mem_free(hook);
        return 0;
    }
    hook->function_address = function_address;
    hook->size = size;

    if (!map_insert(trampoline_hooks_map_, (int)function_address, hook))
    {
        unset_hook_(hook->function_address, hook->trampoline_address,
                    hook->size);
        mem_free(hook);
        return 0;
    }
    return hook->trampoline_address;
}

void hook_unset(void* function_address)
{
    TrampolineHook* hook =
        map_find(trampoline_hooks_map_, (int)function_address);
    if (hook)
    {
        unset_hook_(hook->function_address, hook->trampoline_address,
                    hook->size);
        map_erase(trampoline_hooks_map_, (int)function_address);
        mem_free(hook);
    }
}

void* hook_set_vmt(void** function_address_ptr, void* hook_address)
{
    VmtHook* hook = mem_alloc(sizeof(VmtHook));
    if (!hook)
    {
        return 0;
    }
    hook->orig_function_address =
        set_vmt_hook_(function_address_ptr, hook_address);
    if (!hook->orig_function_address)
    {
        mem_free(hook);
        return 0;
    }
    if (!map_insert(vmt_hooks_map_, (int)hook_address, hook))
    {
        unset_vmt_hook_(function_address_ptr, hook->orig_function_address);
        return 0;
    }
    hook->function_address_ptr = function_address_ptr;
    return hook->orig_function_address;
}

void hook_destroy(void)
{
    map_foreach(trampoline_hooks_map_, callback_unset_trampoline_hooks_);
    map_clear(trampoline_hooks_map_);
    map_destroy(trampoline_hooks_map_);

    map_foreach(vmt_hooks_map_, callback_unset_vmt_hooks_);
    map_clear(vmt_hooks_map_);
    map_destroy(vmt_hooks_map_);
}
