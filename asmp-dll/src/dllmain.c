/**
 * @file dllmain.c
 * @brief Entry point of alien shooter multiplayer client.
 *
 */
#include <windows.h>

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
 * @brief Main thread of the DLL.
 */
static void dllmain_entry(void);

static void dllmain_entry(void)
{
    while (1)
    {
        Sleep(100);
        if (GetAsyncKeyState(VK_F12))
        {
            break;
        }
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
