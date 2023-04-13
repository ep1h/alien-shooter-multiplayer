/**
 * @file EntPlayer.c
 * @brief Implementation of the API for interacting with player entities.
 *
 */
#include "EntPlayer.h"
#include "game/addresses.h"

int EntPlayer__action(Entity* ECX, enEntAction action, void* arg1, void* arg2,
                      void* arg3)
{
    return ((Entity__action_t)FUNC_ENT_PLAYER_ACTION)(ECX, 0, action, arg1,
                                                      arg2, arg3);
}

int EntPlayer__set_armed_weapon(EntPlayer* ECX, int weapon_slot_id)
{
    return ((EntPlayer__set_armed_weapon_t)FUNC_ENT_PLAYER_SET_ARMED_WEAPON)(
        ECX, 0, weapon_slot_id);
}

int EntPlayer__give_weapon(EntPlayer* ECX, enVid weapon_vid_idx)
{
    return ((EntPlayer__give_weapon_t)FUNC_ENT_PLAYER_GIVE_WEAPON)(
        ECX, 0, weapon_vid_idx);
}
