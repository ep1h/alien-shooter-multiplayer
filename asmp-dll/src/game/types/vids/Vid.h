#ifndef VIDS_VID
#define VIDS_VID

#include "../common.h"
#include "../enums.h"

typedef struct Vid Vid;
typedef struct Vid_vtbl Vid_vtbl;
typedef struct Entity Entity;

typedef struct __cppobj Vid
{
    Vid_vtbl* __vftable /*VFT*/;
    int idx;
    char* name;
    enVidSprType type;
    enVidSprClass spriteClass;
    uint32_t field_0x14;
    uint32_t move_mask;
    float field_0x1C;
    float field_0x20;
    float field_0x24;
    uint32_t maxhp;
    float speed;
    uint32_t field_30;
    uint32_t field_34;
    uint32_t field_38;
    uint32_t field_3C;
    uint32_t field_40;
    uint32_t field_44;
    uint32_t damage_after_death;
    uint32_t field_4C;
    uint32_t field_50;
    uint32_t field_54;
    uint32_t field_58;
    Vid* linked_vid;
    uint32_t field_60;
    uint32_t field_64;
    uint32_t field_68;
    uint32_t lifetime_ms;
    uint32_t directions_number;
    uint32_t field_74;
    uint32_t field_78;
    uint32_t field_7C;
    uint32_t field_80[99];
    uint32_t field_20C;
    uint32_t field_210[52];
    float field_2E0;
    float field_2E4;
    float field_2E8;
    uint32_t field_2EC;
    uint32_t frame_speed;
    uint32_t field_2F4[35];
    uint32_t field_380;
    float unk_x_or_w;
    float unk_y_or_h;
    uint32_t field_38C;
    uint32_t field_390;
    uint32_t field_394;
    uint32_t field_398;
    uint32_t field_39C;
    uint32_t field_3A0;
    uint32_t field_3A4;
    uint32_t created_units_0;
    uint32_t created_units_1;
    uint32_t created_units_2;
    uint32_t created_units_3;
    uint32_t deaths_n_0;
    uint32_t deaths_n_1;
    uint32_t deaths_n_2;
    uint32_t deaths_n_3;
    uint32_t recolors_n_0;
    uint32_t recolors_n_1;
    uint32_t recolors_n_2;
    uint32_t recolors_n_3;
    uint32_t max_hp_0;
    uint32_t max_hp_1;
    uint32_t max_hp_2;
    uint32_t max_hp_3;
    uint32_t field_3E8;
    uint32_t field_3EC;
    uint32_t field_3F0[6];
    uint32_t field_408[20];
    uint32_t field_458;
    uint32_t battlerange_or_ptr_to;
    uint32_t field_460;
    uint32_t field_464;
    uint32_t field_468;
    uint32_t field_46C;
    uint32_t field_470;
    uint32_t field_474;
    uint32_t field_478;
    uint32_t field_47C;
    uint32_t can_not_be_child;
    uint32_t field_484;
    uint32_t field_488;
    uint32_t field_48C;
} Vid;

typedef struct Vid_vtbl
{
    Vid*(__thiscall* unk_0)(Vid* __hidden this);
    void(__thiscall* destructor)(Vid* __hidden this, int);
    void* unk_2;
    int(__thiscall* f3)(Vid* __hidden this, Entity*);
    void* unk_4;
    void* unk_5;
    void* unk_6;
    void* unk_7;
    void* unk_8;
    void* unk_9;
} Vid_vtbl;

#endif /* VIDS_VID */
