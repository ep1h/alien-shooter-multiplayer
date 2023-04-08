/**
 * @file EntPlayer.h
 * @brief Provides an API for interacting with player entities.
 *
 */
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

/**
 * @brief Sets currently used weapon slot id.
 *
 * @param[in] ECX            EntPlayer instance pointer.
 * @param[in] weapon_slot_id Weapon slot id.
 *
 * @return 1 Weapon slot id is valid and player entity has a weapon in that
 *             slot.
 * @return 0 Otherwise.
 */
int EntPlayer__set_armed_weapon(EntPlayer* ECX, int weapon_slot_id);

/**
 * @brief Givus a weapon to player entity.
 *
 * @param[in] ECX            EntPlayer instance pointer.
 * @param[in] weapon_vid_idx Vid index of weapon to give.
 *
 * @return 1 If weapon was given.
 * @return 0 Otherwise.
 */
int EntPlayer__give_weapon(EntPlayer* ECX, enVid weapon_vid_idx);

#endif /* ENTITIES_ENTPLAYER_H */
