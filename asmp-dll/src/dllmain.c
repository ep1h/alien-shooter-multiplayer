#include <stdbool.h>
#include <windows.h>
#include "utils/console/console.h"

static HINSTANCE instance_handle = 0;

static bool dllmain_init(void);
static void dllmain_destroy(void);
static void dllmain_loop(void);
static void dllmain_entry(void);

static bool dllmain_init(void)
{
    if (console_init())
    {
        return true;
    }
    return false;
}

static void dllmain_destroy(void)
{
    console_destroy();
}

static void dllmain_loop(void)
{
    while (1)
    {
        Sleep(100);
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
