#ifndef TYPES_SOUND_H
#define TYPES_SOUND_H

#include "common.h"

typedef struct Sound Sound;

typedef float(CC_THISCALL* Sound__set_sound_volume_t)(Sound* this, int volume);
typedef float(CC_THISCALL* Sound__set_music_volume_t)(Sound* this, int volume);

typedef struct Sound
{
    uint32_t field_0[132];
    void* lpDirectSound8;
    uint32_t field_214[119];
    uint32_t field_3F0;
    uint32_t field_3F4;
    uint32_t field_3F8;
    uint32_t unk_volume;
    uint32_t field_400;
    uint32_t sound_volume;
    uint32_t music_volume;
    uint32_t field_40C;
    uint32_t mb_loop;
    uint32_t field_414;
    uint32_t last_418;
} Sound;

#endif /* TYPES_SOUND_H */
