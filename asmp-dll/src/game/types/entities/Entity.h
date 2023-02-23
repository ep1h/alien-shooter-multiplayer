#ifndef ENTITIES_ENTITY_H
#define ENTITIES_ENTITY_H

#include "../common.h"
#include "../enums.h"

typedef struct Entity Entity;
typedef struct Entity_vtbl Entity_vtbl;
typedef struct EntityItem EntityItem;
typedef struct Vid Vid;

typedef float(__thiscall* Entity__calc_screen_x_t)(Entity* this);
typedef float(__thiscall* Entity__calc_screen_y_t)(Entity* this);
typedef float(__thiscall* Entity__get_screen_z_t)(Entity* this);
typedef float(__thiscall* Entity__get_screen_x_t)(Entity* this);
typedef float(__thiscall* Entity__get_screen_y_t)(Entity* this);
typedef Vid*(__thiscall* Entity__get_vid_t)(Entity* this);
typedef Entity*(__thiscall* Entity__get_child_t)(Entity* this);
typedef void*(__thiscall* Entity__destroy_t)(Entity* this, int free);
typedef void(__thiscall* Entity__move_t)(Entity* this, float x, float y,
                                         float z);
typedef int(__thiscall* Entity__action_t)(Entity* this, enEntAction action,
                                          void* a3, void* a4, void* a5);
typedef char(__thiscall* Entity__rotate_t)(Entity* this,
                                           unsigned char direction);
typedef void(__thiscall* Entity__set_anim_t)(Entity* this, enAnim anim_id);
typedef void(__thiscall* Entity__add_child_t)(Entity* this, Entity* child);
typedef void(__thiscall* Entity__set_health_t)(Entity* this, int health);

typedef struct __cppobj Entity
{
    Entity_vtbl* __vftable /*VFT*/;
    uint32_t field_4;
    uint32_t field_8;
    uint32_t field_C;
    uint32_t field_10;
    uint32_t field_14;
    uint32_t field_18;
    Vid* vid;
    float velocity;
    uint32_t field_24;
    uint32_t flag;
    uint32_t refs_number;
    float x;
    float y;
    float z;
    Entity* goal;
    Entity* child;
    Entity* parent;
    enAnim anim;
    uint8_t direction;
    uint8_t unk_4D[3];
    uint32_t field_50;
    uint32_t health;
    uint32_t field_58;
    uint32_t field_5C;
    uint32_t field_60;
    uint32_t field_64;
    EntityItem* items;
    uint32_t field_6C;
} Entity;

typedef struct __cppobj Entity_vtbl
{
    Entity__destroy_t destroy;
    Entity__action_t action;
    void* unk_2;
    void*(__thiscall* f3)(Entity* __hidden this);
    void(__thiscall* f4)(Entity* __hidden this, Entity*);
    void* unk_5;
    int(__thiscall* f6)(Entity* __hidden this);
    void(__thiscall* f7)(Entity* __hidden this);
} Entity_vtbl;

#endif /* ENTITIES_ENTITY_H */
