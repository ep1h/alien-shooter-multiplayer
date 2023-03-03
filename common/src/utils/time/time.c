#include "time.h"
#include <windows.h>

unsigned long time_get_ms(void)
{
    static unsigned long start = 0;
    if (start == 0)
    {
        start = GetTickCount();
    }
    unsigned long cur = GetTickCount();
    return cur - start;
}

void time_sleep_ms(int time_ms)
{
    Sleep(time_ms);
}
