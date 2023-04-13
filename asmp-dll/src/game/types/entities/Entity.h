/**
 * @file Entity.h
 * @brief Provides an API for interacting with base game entity.
 *
 */
#ifndef ENTITIES_ENTITY_H
#define ENTITIES_ENTITY_H

#include "../common.h"
#include "../enums.h"

typedef struct Entity Entity;
typedef struct Entity_vtbl Entity_vtbl;
typedef struct EntityItem EntityItem;
typedef struct Vid Vid;

typedef float(__thiscall* Entity__calc_screen_x_t)(Entity* this);
typedef float(__thiscall* Entity__calc_screen_y_t)(Entity* this);
typedef float(__thiscall* Entity__get_screen_z_t)(Entity* this);
typedef float(__thiscall* Entity__get_screen_x_t)(Entity* this);
typedef float(__thiscall* Entity__get_screen_y_t)(Entity* this);
typedef Vid*(__thiscall* Entity__get_vid_t)(Entity* this);
typedef EntityItem*(__thiscall* Entity__get_items_t)(Entity* this);
typedef Entity*(__thiscall* Entity__get_child_t)(Entity* this);
typedef void*(__thiscall* Entity__destroy_t)(Entity* this, int free);
typedef Entity*(__thiscall* Entity__get_goal_t)(Entity* this);
typedef void(__thiscall* Entity__move_t)(Entity* this, float x, float y,
                                         float z);
typedef int(__thiscall* Entity__action_t)(Entity* this, enEntAction action,
                                          void* arg1, void* arg2, void* arg3);
typedef char(__thiscall* Entity__rotate_t)(Entity* this,
                                           unsigned char direction);
typedef void(__thiscall* Entity__set_anim_t)(Entity* this, enAnim anim_id);
typedef void(__thiscall* Entity__add_child_t)(Entity* this, Entity* child);
typedef int(__thiscall* Entity__remove_child_by_vid_t)(Entity* this, Vid* vid);
typedef int(__thiscall* Entity__play_sfx_t)(Entity* this, int sfx_idx);
typedef void(__thiscall* Entity__set_health_t)(Entity* this, int health);


/**
 * @brief Base entity type. All other entities are derived from this type.
 */
typedef struct __cppobj Entity
{
    Entity_vtbl* __vftable /*VFT*/;
    uint32_t field_4;
    uint32_t field_8;
    uint32_t field_C;
    uint32_t field_10;
    uint32_t field_14;
    uint32_t field_18;
    Vid* vid;
    float velocity;
    uint32_t field_24;
    uint32_t flag;
    uint32_t refs_number;
    float x;
    float y;
    float z;
    Entity* goal;
    Entity* child;
    Entity* parent;
    enAnim anim;
    uint8_t direction;
    uint8_t unk_4D[3];
    uint32_t field_50;
    uint32_t health;
    uint32_t field_58;
    uint32_t field_5C;
    uint32_t field_60;
    uint32_t field_64;
    EntityItem* items;
    uint32_t field_6C;
} Entity;

typedef struct __cppobj Entity_vtbl
{
    Entity__destroy_t destroy;
    Entity__action_t action;
    void* unk_2;
    void*(__thiscall* f3)(Entity* __hidden this);
    void(__thiscall* f4)(Entity* __hidden this, Entity*);
    void* unk_5;
    int(__thiscall* f6)(Entity* __hidden this);
    void(__thiscall* f7)(Entity* __hidden this);
} Entity_vtbl;


/**
 * @brief Calculates screen x coordinate of an Entity.
 *
 * @param[in] ECX Pointer to Entity to calculate the screen x-coordinate for.
 *
 * @return float Screen x coordinate of the entity.
 */
float Entity__calc_screen_x(Entity* ECX);

/**
 * @brief Calculates screen y-coordinate of an Entity.
 *
 * @param[in] ECX Pointer to Entity to calculate the screen y-coordinate for.
 *
 * @return float Screen y coordinate of the entity.
 */
float Entity__calc_screen_y(Entity* ECX);

/**
 * @brief Returns x-coordinate of an Entity.
 *
 * @param[in] ECX Pointer to Entity to get the x-coordinate for.
 *
 * @return float y coordinate of the entity.
 */
float Entity__get_x(Entity* ECX);

/**
 * @brief Returns y-coordinate of an Entity.
 *
 * @param[in] ECX Pointer to Entity to get the y-coordinate for.
 *
 * @return float y coordinate of the entity.
 */
float Entity__get_y(Entity* ECX);

/**
 * @brief Returns z-coordinate of an Entity.
 *
 * @param[in] ECX Pointer to Entity to get the z-coordinate for.
 *
 * @return float z coordinate of the entity.
 */
float Entity__get_z(Entity* ECX);

/**
 * @brief Returns Vid pointer of an Entity.
 *
 * @param[in] ECX Pointer to Entity to get the Vid pointer for.
 *
 * @return Vid* Vid pointer of the entity.
 */
Vid* Entity__get_vid(Entity* ECX);


/**
 * @brief Returns pointer to entity items.
 *
 * @param[in] ECX Pointer to Entity to get the items pointer for.
 *
 * @return EntityItem* Pointer to entity items.
 */
EntityItem* Entity__get_items(Entity* ECX);

/**
 * @brief Returns child Entity pointer of an Entity.
 *
 * @param[in] ECX Pointer to Entity to get the child Entity pointer for.
 *
 * @return Entity* Child Entity pointer of the entity.
 */
Entity* Entity__get_child(Entity* ECX);

/**
 * @brief Destroys an Entity.
 *
 * @param[in] ECX  Pointer to Entity to destroy.
 * @param[in] free Whether to free the memory allocated for Entity.
 *
 * @return void* NULL.
 */
void* Entity__destroy(Entity* ECX, int free);

/**
 * @brief Moves an Entity and all its children.
 *
 * @param[in] ECX Pointer to Entity to move.
 * @param[in] x   X-coordinate to move to.
 * @param[in] y   Y-coordinate to move to.
 * @param[in] z   Z-coordinate to move to.
 */
void Entity__move(Entity* ECX, float x, float y, float z);

/**
 * @brief Executes an action for an Entity.
 *
 * @param[in] ECX     Pointer to Entity to execute action for.
 * @param[in] action  Action to execute.
 * @param[in] arg1    The first action argument.
 * @param[in] arg2    The second action argument.
 * @param[in] arg3    The third action argument.
 *
 * @return int Action execution result.
 */
int Entity__action(Entity* ECX, enEntAction action, void* arg1, void* arg2,
                   void* arg3);

/**
 * @brief Returns goal Entity pointer.
 *
 * @param[in] ECX Pointer to Entity to get the goal Entity pointer for.
 *
 * @return Entity* Goal of the entity.
 */
Entity* Entity__get_goal(Entity* ECX);

/**
 * @brief Rotates an Entity.
 *
 * @param[in] ECX       Pointer to Entity to rotate.
 * @param[in] direction Direction to rotate to.
 */
char Entity__rotate(Entity* ECX, uint8_t direction);

/**
 * @brief Sets animation for an Entity.
 *
 * @param[in] ECX     Pointer to Entity to set animation for.
 * @param[in] anim_id Animation ID to set.
 */
void Entity__set_anim(Entity* ECX, enAnim anim_id);

/**
 * @brief Adds a child Entity to begin of Entity's child list.
 *
 * @param[in] ECX   Pointer to Entity to add child to.
 * @param[in] child Pointer to child Entity to add.
 */
void Entity__add_child_begin(Entity* ECX, Entity* child);

/**
 * @brief Adds a child Entity to end of Entity's child list.
 *
 * @param[in] ECX   Pointer to Entity to add child to.
 * @param[in] child Pointer to child Entity to add.
 */
void Entity__add_child_end(Entity* ECX, Entity* child);

/**
 * @brief Removes a child Entity with specified Vid pointer from Entity's child
 *        list.
 *
 * Removes the first child entity with a matching vid member from the list of
 * child entities belonging to the current entity object, updates the parent and
 * sibling pointers, and calls the child entity's destructor.
 *
 * @param[in] ECX Pointer to Entity to remove child from.
 * @param[in] vid Vid pointer of child Entity to remove.
 *
 * @return 1 if a child sprite was successfully removed
 * @return 0 otherwise
 */
int Entity__remove_child_by_vid(Entity* ECX, Vid* vid);

/**
 * @brief Plays sound effect at the entity's position.
 *
 * @param[in] ECX     Pointer to Entity to play sound effect at.
 * @param[in] sfx_idx Sound effect index to play.
 */
int Entity__play_sfx(Entity* ECX, int sfx_idx);

/**
 * @brief Sets health for an Entity.
 *
 * @param[in] ECX   Pointer to Entity to set health for.
 * @param[in] health Health to set.
 */
void Entity__set_health(Entity* ECX, int health);

#endif /* ENTITIES_ENTITY_H */
