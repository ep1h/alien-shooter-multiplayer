/**
 * @file gameutils.h
 * @brief Provides high-level utility functions for the game, simplifying and
 *        combining multiple game API calls.
 *
 * Offers composite operations that streamline the process of performing common
 * game actions. By consolidating sequences of game API function calls, these
 * utilities provide a more efficient and intuitive way to interact with the
 * game's core functionality.
 *
 */
#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <stdint.h>
#include "game/types/enums.h"

typedef struct Entity Entity;
Entity* gameutils_get_menu_item(enVid vid_idx, uint8_t direction);

#endif /* GAMEUTILS_H */
