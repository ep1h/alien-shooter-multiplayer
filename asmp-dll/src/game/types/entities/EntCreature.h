#ifndef ENTITIES_ENTCREATURE_H
#define ENTITIES_ENTCREATURE_H

#include "../common.h"
#include "EntUnit.h"

typedef struct __cppobj EntCreature
{
    EntUnit ent_unit;
    uint32_t field_90;
    Entity* entity_94;
} EntCreature;

#endif /* ENTITIES_ENTCREATURE_H */
