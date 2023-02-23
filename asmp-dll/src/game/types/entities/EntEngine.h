#ifndef ENTITIES_ENTENGINE_H
#define ENTITIES_ENTENGINE_H

#include "../common.h"
#include "EntUnit.h"

typedef struct __cppobj EntEngine
{
    EntUnit ent_unit;
    uint32_t field_90;
    uint32_t field_94;
    uint32_t field_98;
    Entity* entity_9C;
    uint32_t array_A0[648];
} EntEngine;

#endif /* ENTITIES_ENTENGINE_H */
