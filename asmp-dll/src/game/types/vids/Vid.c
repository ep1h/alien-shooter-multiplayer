/**
 * @file Vid.c
 * @brief Implementation of the API for interacting with vids.
 *
 */
#include "Vid.h"
#include "addresses.h"
#include "game/types/common.h"

Vid* Vid__set_hidden(Vid* ECX, int value)
{
    return ((Vid__set_hidden_t)FUNC_VID_SET_HIDDEN)(ECX, 0, value);
}

uint32_t Vid__set_hp_coeff_army(Vid* ECX, char army, int coeff)
{
    return ((Vid__set_hp_coeff_army_t)FUNC_VID_SET_HP_COEFF_ARMY)(ECX, 0, army,
                                                                  coeff);
}

Vid* Vid__set_max_hp_army(Vid* ECX, char army, int maxhp)
{
    return ((Vid__set_max_hp_army_t)FUNC_VID_SET_MAX_HP_ARMY)(ECX, 0, army,
                                                              maxhp);
}

uint32_t Vid__is_hidden(Vid* ECX)
{
    return ((Vid__is_hidden_t)FUNC_VID_IS_HIDDEN)(ECX, 0);
}

int Vid__get_ammo(Vid* ECX)
{
    return ((Vid__get_ammo_t)FUNC_VID_GET_AMMO)(ECX, 0);
}

uint32_t Vid__get_max_hp_army(Vid* ECX, char army_idx)
{
    return ((Vid__get_max_hp_army_t)FUNC_VID_GET_MAX_HP_ARMY)(ECX, 0, army_idx);
}

uint32_t Vid__get_entities_number_army(Vid* ECX, int army_idx)
{
    return ((Vid__get_entities_number_army_t)FUNC_VID_GET_ENTITIES_NUMBER_ARMY)(
        ECX, 0, army_idx);
}
uint32_t Vid__get_entities_number_total(Vid* ECX)
{
    return (
        (Vid__get_entities_number_total_t)FUNC_VID_GET_ENTITIES_NUMBER_TOTAL)(
        ECX, 0);
}

uint32_t Vid__get_death_number_army(Vid* ECX, int army_idx)
{
    return ((Vid__get_death_army_t)FUNC_VID_GET_DEATHS_NUMBER_ARMY)(ECX, 0,
                                                                    army_idx);
}

uint32_t Vid__get_death_total(Vid* ECX)
{
    return ((Vid__get_death_total_t)FUNC_VID_GET_DEATHS_NUMBER_TOTAL)(ECX, 0);
}

uint32_t Vid__get_recolors_number_army(Vid* ECX, char army_idx)
{
    return ((Vid__get_recolors_number_army_t)FUNC_VID_GET_RECOLORS_NUMBER_ARMY)(
        ECX, 0, army_idx);
}

uint32_t Vid__get_recolors_number_total(Vid* ECX)
{
    return (
        (Vid__get_recolors_number_total_t)FUNC_VID_GET_RECOLORS_NUMBER_TOTAL)(
        ECX, 0);
}

uint32_t Vid__get_can_not_be_child(Vid* ECX)
{
    return ((Vid__get_can_not_be_child_t)FUNC_VID_GET_CAN_NOT_BE_A_CHILD)(ECX,
                                                                          0);
}

uint32_t Vid__set_can_not_be_child(Vid* ECX, uint32_t a2)
{
    return ((Vid__set_can_not_be_child_t)FUNC_VID_SET_CAN_NOT_BE_A_CHILD)(
        ECX, 0, a2);
}
