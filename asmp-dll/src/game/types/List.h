#ifndef TYPES_LISTS_H
#define TYPES_LISTS_H

#include "common.h"

typedef struct List List;
typedef struct List_vtbl List_vtbl;
typedef struct ListMenu ListMenu;
typedef struct Entity Entity;

typedef List*(__thiscall* List__destroy_t)(List* this, char free);

typedef struct __cppobj List
{
    List_vtbl* __vftable /*VFT*/;
    uint32_t items_number;
    uint32_t max_items;
    void** data;
} List;

typedef struct List_vtbl
{
    List__destroy_t destroy;
} List_vtbl;

typedef int(__thiscall* ListMenu__get_hovered_vid_idx_t)(ListMenu* this);
typedef unsigned int(__thiscall* ListMenu__get_hovered_dir_t)(ListMenu* this);
typedef int(__thiscall* List__remove_item_by_ptr_t)(List* this, Entity* ptr);

typedef struct __cppobj ListMenu
{
    List list;
    uint32_t click_state;
    Entity* ent_hovered;
} ListMenu;

typedef struct __cppobj PairedLists
{
    List first;
    List second;
} PairedLists;

#endif /* TYPES_LISTS_H */
