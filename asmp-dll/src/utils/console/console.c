/**
 * @file console.c
 * @brief Implementation of console component.
*/
#include "console.h"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static bool is_inited;
static bool console_existed_before;
static FILE* console_file;

/**
 * @brief Creates a console window.
 *
 * @return true if succeeded, false otherwise.
 */
static bool create_console_(void)
{
    extern bool console_existed_before;
    bool result = false;
    if (AllocConsole())
    {
        result = true;
    }
    else
    {
        if (GetLastError() == ERROR_ACCESS_DENIED)
        {
            console_existed_before = true;
            result = true;
        }
    }
    return result;
}

/**
 * @brief Redirects stdout to the console window.
 *
 * @return true if succeeded, false otherwise.
 */
static bool redirect_output_(void)
{
    bool result = false;
    if (console_file)
    {
        fclose(console_file);
    }
    if (freopen_s(&console_file, "CONOUT$", "w", stdout) == 0)
    {
        result = true;
    }
    return result;
}

bool console_init(void)
{
    bool result = false;
    if (is_inited)
    {
        result = true;
    }
    else
    {
        if (create_console_())
        {
            if (redirect_output_())
            {
                result = true;
            }
        }
    }
    is_inited = result;
    return result;
}

void console_destroy(void)
{
    if (is_inited && !console_existed_before)
    {
        fclose(console_file);
        FreeConsole();
        is_inited = false;
    }
}

void console_log(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
