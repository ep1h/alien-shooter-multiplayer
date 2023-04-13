/**
 * @file Entity.c
 * @brief Implementation of the API for interacting with base game entity.
 *
 */
#include "Entity.h"
#include "game/addresses.h"
#include "game/types/common.h"


float Entity__calc_screen_x(Entity* ECX)
{
    return ((Entity__calc_screen_x_t)FUNC_ENTITY_CALC_SCREEN_X)(ECX, 0);
}

float Entity__calc_screen_y(Entity* ECX)
{
    return ((Entity__calc_screen_y_t)FUNC_ENTITY_CALC_SCREEN_Y)(ECX, 0);
}

float Entity__get_x(Entity* ECX)
{
    return ((Entity__get_screen_y_t)FUNC_ENTITY_GET_X)(ECX, 0);
}

float Entity__get_y(Entity* ECX)
{
    return ((Entity__get_screen_y_t)FUNC_ENTITY_GET_Y)(ECX, 0);
}

float Entity__get_z(Entity* ECX)
{
    return ((Entity__get_screen_y_t)FUNC_ENTITY_GET_Z)(ECX, 0);
}

Vid* Entity__get_vid(Entity* ECX)
{
    return ((Entity__get_vid_t)FUNC_ENTITY_GET_VID)(ECX, 0);
}

EntityItem* Entity__get_items(Entity* ECX)
{
    return ((Entity__get_items_t)FUNC_ENTITY_GET_ITEMS)(ECX, 0);
}

Entity* Entity__get_child(Entity* ECX)
{
    return ((Entity__get_child_t)FUNC_ENTITY_GET_CHILD)(ECX, 0);
}

void* Entity__destroy(Entity* ECX, int free)
{
    return ((Entity__destroy_t)FUNC_ENTITY_DESTROY)(ECX, 0, free);
}

void Entity__move(Entity* ECX, float x, float y, float z)
{
    return ((Entity__move_t)FUNC_ENTITY_MOVE)(ECX, 0, x, y, z);
}

int Entity__action(Entity* ECX, enEntAction action, void* arg1, void* arg2,
                   void* arg3)
{
    return ((Entity__action_t)FUNC_ENTITY_ACTION)(ECX, 0, action, arg1, arg2,
                                                  arg3);
}

Entity* Entity__get_goal(Entity* ECX)
{
    return ((Entity__get_goal_t)FUNC_ENTITY_GET_GOAL)(ECX, 0);
}

char Entity__rotate(Entity* ECX, uint8_t direction)
{
    return ((Entity__rotate_t)FUNC_ENTITY_ROTATE)(ECX, 0, direction);
}

void Entity__set_anim(Entity* ECX, enAnim anim_id)
{
    return ((Entity__set_anim_t)FUNC_ENTITY_SET_ANIM)(ECX, 0, anim_id);
}

void Entity__add_child_begin(Entity* ECX, Entity* child)
{
    return ((Entity__add_child_t)FUNC_ENTITY_ADD_CHILD_BEGIN)(ECX, 0, child);
}

void Entity__add_child_end(Entity* ECX, Entity* child)
{
    return ((Entity__add_child_t)FUNC_ENTITY_ADD_CHILD_END)(ECX, 0, child);
}

int Entity__remove_child_by_vid(Entity* ECX, Vid* vid)
{
    return ((Entity__remove_child_by_vid_t)FUNC_ENTITY_REMOVE_CHILD_BY_VID)(
        ECX, 0, vid);
}

int Entity__play_sfx(Entity* ECX, int sfx_idx)
{
    return ((Entity__play_sfx_t)FUNC_ENTITY_PLAY_SFX)(ECX, 0, sfx_idx);
}

void Entity__set_health(Entity* ECX, int health)
{
    return ((Entity__set_health_t)FUNC_ENTITY_SET_HEALTH)(ECX, 0, health);
}
