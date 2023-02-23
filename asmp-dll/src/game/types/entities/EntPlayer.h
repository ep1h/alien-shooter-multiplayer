#ifndef ENTITIES_ENTPLAYER_H
#define ENTITIES_ENTPLAYER_H

#include "../common.h"
#include "EntUnit.h"

typedef struct EntPlayer EntPlayer;

typedef int(__thiscall* EntPlayer__set_armed_weapon_t)(EntPlayer* this,
                                                       int weapon_slot_id);

typedef int(__thiscall* EntPlayer__give_weapon_t)(EntPlayer* this,
                                                  enVid weapon_vid_idx);

typedef struct __cppobj EntPlayer
{
    EntUnit ent_unit;
    uint32_t ammo[9];
    uint32_t field_B4;
} EntPlayer;

#endif /* ENTITIES_ENTPLAYER_H */
