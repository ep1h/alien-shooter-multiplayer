#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <stdint.h>
#include "game/types/enums.h"

typedef struct Entity Entity;
Entity* gameutils_get_menu_item(enVid vid_idx, uint8_t direction);

#endif /* GAMEUTILS_H */
