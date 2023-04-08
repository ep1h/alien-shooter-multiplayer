/**
 * @file globals.c
 * @brief Implementation of the API for interacting with global game data and
 *        functions.
 *
 */
#include "globals.h"
#include "game/addresses.h"


Game* game_globals_get_game(void)
{
    return *(Game**)GAME_PTR_PTR;
}

Render* game_globals_get_render(void)
{
    return *(Render**)RENDER_PTR_PTR;
}

HashMap* game_globals_get_hashmap(void)
{
    return *(HashMap**)HASHMAP_PTR_PTR;
}

unsigned long game_globals_get_time_ms(void)
{
    return *(unsigned long*)TIME_MS_PTR;
}
