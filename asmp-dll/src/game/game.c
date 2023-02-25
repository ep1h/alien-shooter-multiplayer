#include "game.h"
#include "addresses.h"

load_menu_t load_menu = (load_menu_t)FUNC_LOAD_MENU;
Entity__set_anim_t Entity__set_anim = (Entity__set_anim_t)FUNC_ENTITY_SET_ANIM;

Game* game_get_game(void)
{
    return *(Game**)GAME_PTR_PTR;
}

Render* game_get_render(void)
{
    return *(Render**)RENDER_PTR_PTR;
}

HashMap* game_get_hashmap(void)
{
    return *(HashMap**)HASHMAP_PTR_PTR;
}
