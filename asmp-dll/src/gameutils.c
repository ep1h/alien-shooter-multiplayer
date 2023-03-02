/**
 * @file gameutils.c
 * @brief Implementation of gameutils component.
 *
 */
#include "gameutils.h"
#include "game/game.h"
#include "game/types/entities/Entity.h"
#include "game/types/List.h"
#include "game/types/vids/Vid.h"

Entity* gameutils_get_menu_item(enVid vid_idx, uint8_t direction)
{
    Game* g = game_get_game();
    if (g)
    {
        ListMenu* lm = &g->list_menu;
        for (enVid i = 0; i < lm->list.items_number; i++)
        {
            Entity* e = ((Entity*)(lm->list.data[i]));
            if (e && e->direction == direction && e->vid &&
                e->vid->idx == (int)vid_idx)
            {
                return e;
            }
        }
    }
    return 0;
}
