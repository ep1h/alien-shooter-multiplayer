#ifndef ENTITIES_ENTREGION_H
#define ENTITIES_ENTREGION_H

#include "../common.h"
#include "Entity.h"

typedef struct __cppobj EntRegion
{
    Entity entity;
    uint32_t field_70;
    uint32_t field_74;
    uint32_t field_78;
    uint32_t field_7C;
    uint32_t field_80;
    uint32_t field_84;
    uint32_t field_88;
    uint32_t field_8C;
    float some_x;
    float some_y;
    uint32_t field_98;
    uint32_t field_9C;
    uint32_t array_A0[12];
} EntRegion;

#endif /* ENTITIES_ENTREGION_H */
