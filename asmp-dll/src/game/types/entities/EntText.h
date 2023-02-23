#ifndef ENTITIES_ENTTEXT_H
#define ENTITIES_ENTTEXT_H

#include "../common.h"
#include "EntFrame.h"

typedef struct __cppobj EntText
{
    EntFrame entity;
    char* text_70;
    char* text_74;
    uint32_t text_len_78;
    uint32_t field_7C;
    uint32_t field_80;
    uint32_t field_84;
} EntText;

#endif /* ENTITIES_ENTTEXT_H */
