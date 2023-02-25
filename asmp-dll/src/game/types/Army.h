#ifndef TYPES_ARMY_H
#define TYPES_ARMY_H

#include "common.h"
#include "List.h"

typedef struct Army Army;
typedef struct Army_vtbl Army_vtbl;
typedef struct EntPlayer EntPlayer;

typedef EntPlayer*(__thiscall* Army__get_player_t)(Army* this);

typedef struct __cppobj Army
{
    Army_vtbl* __vftable /*VFT*/;
    uint32_t money;
    uint32_t field_8;
    uint32_t idx;
    EntPlayer* player;
    List list;
    uint32_t field_24;
    uint32_t field_28;
    uint32_t field_2C;
    uint32_t field_30;
    uint32_t field_34;
    uint32_t field_38;
    uint32_t field_3C;
    uint32_t field_40;
    uint32_t field_44;
    uint32_t field_48;
    uint32_t field_4C;
    uint32_t field_50[183];
} Army;

typedef struct __cppobj Army_vtbl
{
    Army*(__thiscall* destroy)(Army* __hidden this, int free);
    void* f1;
    void* f2;
    void(__thiscall* f3)(Army* __hidden this, int);
    void* f4;
    void* f5;
    void* f6;
    void(__thiscall* f7)(Army* __hidden this);
    void(__thiscall* f8)(Army* __hidden this);
} Army_vtbl;

#endif /* TYPES_ARMY_H */
