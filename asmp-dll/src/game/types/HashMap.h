#ifndef TYPES_HASHMAP_H
#define TYPES_HASHMAP_H

#include "common.h"
#include "List.h"

typedef struct HashMap
{
    uint32_t l_idx;
    uint32_t t_idx;
    uint32_t r_idx;
    uint32_t b_idx;
    uint32_t field_10;
    uint32_t field_14;
    uint32_t unk_mb_list_item_idx;
    uint32_t field_1C;
    int cells_n_x;
    int cells_n_y;
    float cell_w;
    float cell_h;
    List* list_ptr;
    List list;
} HashMap;

#endif /* TYPES_HASHMAP_H */
