#ifndef UTILS_CONSOLE_CONSOLE_H
#define UTILS_CONSOLE_CONSOLE_H

#include <stdbool.h>

bool console_init(void);
void console_destroy(void);
void console_log(const char* format, ...);

#endif /* UTILS_CONSOLE_CONSOLE_H */
