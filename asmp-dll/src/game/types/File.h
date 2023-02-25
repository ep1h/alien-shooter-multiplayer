#ifndef TYPES_FILE_H
#define TYPES_FILE_H

#include "common.h"

typedef struct File File;
typedef struct File_vtbl File_vtbl;
typedef struct FILE FILE;

typedef File*(__thiscall* File__destroy_t)(File* this, char free);
typedef size_t*(__thiscall* File__read_t)(File* this, void* buf, size_t size);
typedef size_t*(__thiscall* File__write_t)(File* this, void* buf, size_t size);

typedef struct __cppobj File
{
    File_vtbl* __vftable /*VFT*/;
    uint32_t field_4;
    uint32_t field_8;
    uint32_t mb_filename;
    uint32_t field_10;
    uint32_t field_14;
    uint32_t field_18;
    uint32_t field_1C;
    uint32_t field_20;
    uint32_t field_24;
    uint32_t field_28;
    uint32_t field_2C;
    uint32_t field_30;
    uint32_t field_34;
    FILE* stream;
    uint32_t field_3C;
} File;

typedef struct __cppobj /*VFT*/ File_vtbl
{
    File__destroy_t destroy;
    File__read_t read;
    File__write_t write;
} File_vtbl;

#endif /* TYPES_FILE_H */
