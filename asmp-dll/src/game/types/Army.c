/**
 * @file Army.c
 * @brief Implementation of the API for interacting with armies.
 *
 */
#include "Army.h"
#include "addresses.h"
#include "game/types/common.h"

Army* Army__destroy(Army* ECX, int free)
{
    return ((Army__destroy_t)FUNC_ARMY_DESTROY)(ECX, 0, free);
}

uint32_t Army__get_money(Army* ECX)
{
    return ((Army__get_money_t)FUNC_ARMY_GET_MONEY)(ECX, 0);
}

uint32_t Army__set_money(Army* ECX, uint32_t money)
{
    return ((Army__set_money_t)FUNC_ARMY_SET_MONEY)(ECX, 0, money);
}

uint32_t Army__set_clever_attack(Army* ECX, int value)
{
    return ((Army__set_clever_attack_t)FUNC_ARMY_SET_CLEVER_ATTACK)(ECX, 0,
                                                                    value);
}

EntPlayer* Army__get_player(Army* ECX)
{
    return ((Army__get_player_t)FUNC_ARMY_GET_PLAYER)(ECX, 0);
}
