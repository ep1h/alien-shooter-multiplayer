#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "types/Game.h"
#include "types/HashMap.h"
#include "types/Render.h"
#include "types/entities/Entity.h"

typedef int(__stdcall* load_menu_t)(const char** menu_file);

extern Entity__set_anim_t Entity__set_anim;

Game* game_get_game(void);
Render* game_get_render(void);
HashMap* game_get_hashmap(void);

#endif /* GAME_GAME_H */
