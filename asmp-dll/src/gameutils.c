/**
 * @file gameutils.c
 * @brief Implementation of gameutils component.
 *
 */
#include "gameutils.h"
#include "game/game.h"
#include "game/types/Army.h"
#include "game/types/entities/Entity.h"
#include "game/types/entities/EntPlayer.h"
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

void gameutils_spawn_weapons(void)
{
    Game* g = game_get_game();
    if (g)
    {
        Army* a = g->army[g->mb_flagman_army];
        if (a)
        {
            Entity* p = (Entity*)a->player;
            if (p)
            {
                float x = p->x;
                float y = p->y;
                float z = p->z;
                x += 100.0f;
                for (int i = 262; i <= 269; i++)
                {
                    g->__vftable->create_entity(g, 0, g->vids[i], x, y, z, 0,
                                                0);
                    x += 50.0f;
                }
            }
        }
    }
}

EntPlayer* gameutils_get_player(void)
{
    Game* g = game_get_game();
    if (g)
    {
        Army* a = g->army[g->mb_flagman_army];
        if (a)
        {
            return a->player;
        }
    }
    return 0;
}

Entity* gameutils_create_entity(Vid* vid, float x, float y, float z)
{
    Game* g = game_get_game();
    if (g)
    {
        return g->__vftable->create_entity(g, 0, vid, x, y, z, 0, 0);
    }
    return 0;
}
