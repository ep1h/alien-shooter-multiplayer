/**
 * @file globals.h
 * @brief Provides an API for interacting with global game data and functions.
 *
 */
#ifndef GAME_GLOBALS_H
#define GABE_GLOBALS_H

#include "types/common.h"

typedef struct Game Game;
typedef struct Render Render;
typedef struct HashMap HashMap;

typedef int(CC_STDCALL* load_menu_t)(const char** menu_file);

/**
 * @brief Returns a pointer to the main game class.
 *
 * @return Game*
 */
Game* game_globals_get_game(void);

/**
 * @brief Returns a pointer to the game renderer class.
 *
 * @return Render*
 */
Render* game_globals_get_render(void);

/**
 * @brief Returns a pointer to the game's global hashmap with all created
 *        entities.
 *
 * @return HashMap*
 */
HashMap* game_globals_get_hashmap(void);

/**
 * @brief Returns current time in milliseconds.
 *
 * @return unsigned long time in milliseconds.
 */
unsigned long game_globals_get_time_ms(void);

#endif /* GAME_GLOBALS_H */
