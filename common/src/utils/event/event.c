#include "event.h"
#include <windows.h>
#include "utils/mem/mem.h"

typedef struct Event
{
    int trigger;
} Event;

Event* event_create(void)
{
    Event* event = mem_alloc(sizeof(Event));
    if (!event)
    {
        return 0;
    }
    event->trigger = 0;
    return event;
}

void event_destroy(Event* event)
{
    if (!event)
    {
        return;
    }
    mem_free(event);
}

void event_wait(Event* event)
{
    if (!event)
    {
        return;
    }
    WaitOnAddress(&event->trigger, &event->trigger, sizeof(event->trigger),
                  INFINITE);
}

bool event_wait_timeout(Event* event, int timeout_ms)
{
    if (!event)
    {
        return false;
    }
    if (event->trigger != 0)
    {
        event->trigger = 0;
        return true;
    }
    if (!WaitOnAddress(&event->trigger, &event->trigger, sizeof(event->trigger),
                       timeout_ms))
    {
        if (GetLastError() == ERROR_TIMEOUT)
        {
            return false;
        }
    }
    return true;
}

void event_trigger(Event* event)
{
    if (!event)
    {
        return;
    }
    event->trigger = 1;
    WakeByAddressSingle(&event->trigger);
}
