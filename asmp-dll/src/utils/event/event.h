#ifndef UTILS_EVENT_EVENT_H
#define UTILS_EVENT_EVENT_H

#include <stdbool.h>

typedef struct Event Event;

Event* event_create(void);
void event_destroy(Event* event);
void event_wait(Event* event);
bool event_wait_timeout(Event* event, int timeout_ms);
void event_trigger(Event* event);

#endif /* UTILS_EVENT_EVENT_H */
