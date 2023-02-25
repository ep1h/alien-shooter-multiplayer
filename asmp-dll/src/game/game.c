#include "game.h"
#include "addresses.h"

load_menu_t load_menu = (load_menu_t)FUNC_LOAD_MENU;
Game__load_map_t Game__load_map = (Game__load_map_t)FUNC_GAME_LOAD_MAP;
Render__draw_colored_rect_t Render__draw_colored_rect =
    (Render__draw_colored_rect_t)FUNC_RENDER_DRAW_COLORED_RECT;
Entity__move_t Entity__move = (Entity__move_t)FUNC_ENTITY_MOVE;
Entity__rotate_t Entity__rotate = (Entity__rotate_t)FUNC_ENTITY_ROTATE;
Entity__set_anim_t Entity__set_anim = (Entity__set_anim_t)FUNC_ENTITY_SET_ANIM;
EntPlayer__set_armed_weapon_t EntPlayer__set_armed_weapon =
    (EntPlayer__set_armed_weapon_t)FUNC_ENT_PLAYER_SET_ARMED_WEAPON;
List__remove_item_by_ptr_t List__remove_item_by_ptr =
    (List__remove_item_by_ptr_t)FUNC_LIST_REMOVE_ITEM_BY_DATA;
Entity__add_child_t Entity__add_child_begin =
    (Entity__add_child_t)FUNC_ENTITY_ADD_CHILD_BEGIN;
Entity__add_child_t Entity__add_child_end =
    (Entity__add_child_t)FUNC_ENTITY_ADD_CHILD_END;
Entity__set_health_t Entity__set_health =
    (Entity__set_health_t)FUNC_ENTITY_SET_HEALTH;

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

uint32_t game_get_time_ms(void)
{
    return *(uint32_t*)TIME_MS_PTR;
}
