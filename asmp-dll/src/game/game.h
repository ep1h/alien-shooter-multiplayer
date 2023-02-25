#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "types/Game.h"
#include "types/HashMap.h"
#include "types/Render.h"
#include "types/entities/EntPlayer.h"

typedef int(__stdcall* load_menu_t)(const char** menu_file);

extern Game__load_map_t Game__load_map;
extern Render__draw_colored_rect_t Render__draw_colored_rect;
extern Entity__move_t Entity__move;
extern Entity__rotate_t Entity__rotate;
extern Entity__set_anim_t Entity__set_anim;
extern EntPlayer__set_armed_weapon_t EntPlayer__set_armed_weapon;
extern List__remove_item_by_ptr_t List__remove_item_by_ptr;
extern Entity__add_child_t Entity__add_child_begin;
extern Entity__add_child_t Entity__add_child_end;
extern Entity__set_health_t Entity__set_health;

Game* game_get_game(void);
Render* game_get_render(void);
HashMap* game_get_hashmap(void);
uint32_t game_get_time_ms(void);

#endif /* GAME_GAME_H */
