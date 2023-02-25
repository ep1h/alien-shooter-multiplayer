/**
 * @file console.h
 * @brief Provides API to initialize/destroy a console, redirect output to it,
 *        and log messages.
 */
#ifndef UTILS_CONSOLE_CONSOLE_H
#define UTILS_CONSOLE_CONSOLE_H

#include <stdbool.h>

/**
 * @brief Initializes the console.
 *
 * @return true if succeeded, false otherwise.
 */
bool console_init(void);

/**
 * @brief Destroys the console.
 */
void console_destroy(void);

/**
 * @brief Logs a message to the console.
 *
 * @param[in] format Format string.
 * @param[in] ...    Format arguments.
 */
void console_log(const char* format, ...);

#endif /* UTILS_CONSOLE_CONSOLE_H */
