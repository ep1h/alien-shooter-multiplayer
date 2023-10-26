#ifndef TYPES_INPUT_H
#define TYPES_INPUT_H

#include "common.h"
#include "enums.h"

typedef struct Input Input;
typedef struct File File;

typedef int(CC_THISCALL* Inpit__Input_t)(Input* this);
typedef size_t(CC_THISCALL* Input__write_to_file)(Input* this, File* file);
typedef size_t(CC_THISCALL* Input__read_from_file)(Input* this, File* file);
typedef int(CC_THISCALL* Inpit__clear_t)(Input* this);

typedef struct Input
{
    enInput button;
    uint32_t mouse_wheel;
    uint32_t cursor_x_global;
    uint32_t cursor_y_global;
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t field_18;
    uint32_t field_1C;
} Input;

#endif /* TYPES_INPUT_H */
