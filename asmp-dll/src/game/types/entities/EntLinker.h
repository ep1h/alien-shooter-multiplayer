#ifndef ENTITIES_ENTLINKER_H
#define ENTITIES_ENTLINKER_H

#include "../common.h"
#include "Entity.h"

typedef struct __cppobj EntLinker
{
    Entity entity;
    uint32_t field_70;
    uint32_t field_74;
    uint32_t field_78;
    uint32_t field_7C;
    Entity* parent;
} EntLinker;

#endif /* ENTITIES_ENTLINKER_H */
