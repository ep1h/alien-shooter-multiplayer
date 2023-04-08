/**
 * @file Game.c
 * @brief Implementation of the API for interacting with the main game class.
 *
 */
#include "Game.h"
#include "game/addresses.h"

void Game__load_map(Game* ECX, const char* map_file_path)
{
    return ((Game__load_map_t)FUNC_GAME_LOAD_MAP)(ECX, 0, map_file_path);
}
