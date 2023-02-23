#ifndef ENTITIES_ENTOBJECT_H
#define ENTITIES_ENTOBJECT_H

#include "../common.h"
#include "Entity.h"

typedef struct __cppobj EntObject
{
    Entity entity;
    uint32_t field_70;
    uint32_t field_74;
} EntObject;

#endif /* ENTITIES_ENTOBJECT_H */
