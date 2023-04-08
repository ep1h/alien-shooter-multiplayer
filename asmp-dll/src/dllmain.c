/**
 * @file dllmain.c
 * @brief Entry point of alien shooter multiplayer client.
 *
 */
#include <stdbool.h>
#include <windows.h>
#include "utils/console/console.h"
#include "utils/hook/hook.h"
#include "multiplayer.h"
#include "gameutils.h"
#include "game/api.h"


static HINSTANCE instance_handle = 0;


/**
 * @brief Entry point into a DLL.
 *
 * @param[in] hinst    Base address of the DLL.
 * @param[in] reason   The reason for calling the DLL entry point.
 * @param[in] reserved For reason = DLL_PROCESS_ATTACH: NULL for dynamic loads
 *                     and non-NULL for static loads.
 *                     For reason = DLL_PROCESS_DETACH: NULL if FreeLibrary has
 *                     been called or the DLL load failed and non-NULL if the
 *                     process is terminating.
 *
 * @return Injection/uninjection result.
 */
BOOL WINAPI DllMain(HMODULE hinst, DWORD reason, LPVOID reserved);

/**
 * @brief Initialization of the components required for the DLL to work.
 *
 * @return true
 * @return false
 */
static bool dllmain_init(void);

/**
 * @brief Cleanup logic before DLL unloading.
 */
static void dllmain_destroy(void);

/**
 * @brief Main loop of the DLL.
 */
static void dllmain_loop(void);

/**
 * @brief Main thread of the DLL.
 */
static void dllmain_entry(void);

static bool dllmain_init(void)
{
    if (console_init())
    {
        if (hook_init())
        {
            if (multiplayer_init())
            {
                return true;
            }
            hook_destroy();
        }
        console_destroy();
    }
    return false;
}

static void dllmain_destroy(void)
{
    multiplayer_destroy();
    hook_destroy();
    console_destroy();
}

static void dllmain_loop(void)
{
    while (1)
    {
        Sleep(100);
        if (GetAsyncKeyState(VK_F6))
        {
            gameutils_spawn_weapons();
            Sleep(1000);
        }
        if (GetAsyncKeyState(VK_F12))
        {
            break;
        }
    }
}

static void dllmain_entry(void)
{
    if (dllmain_init())
    {
        console_log("dllmain_entry()\n");
        dllmain_loop();
        dllmain_destroy();
    }
    FreeLibraryAndExitThread(instance_handle, 0);
}

BOOL WINAPI DllMain(HMODULE hinst, DWORD reason, LPVOID reserved)
{
    (void)reserved;
    instance_handle = hinst;
    if (reason == DLL_PROCESS_ATTACH)
    {
        DWORD thread_id = 0;
        CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dllmain_entry,
                                 0x0, 0, &thread_id));
    }
    return TRUE;
}
