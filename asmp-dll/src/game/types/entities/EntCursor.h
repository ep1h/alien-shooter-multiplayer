#ifndef ENTITIES_ENTCURSOR_H
#define ENTITIES_ENTCURSOR_H

#include "../common.h"
#include "Entity.h"

typedef struct __cppobj EntCursor
{
    Entity entity;
    uint32_t unk_70;
    uint32_t mb_HCURSOR;
    uint32_t unk_78;
    uint32_t unk_7C;
    uint32_t unk_80[34];
} EntCursor;

#endif /* ENTITIES_ENTCURSOR_H */
