/**
 * @file List.c
 * @brief Implementation of the API for interacting with game lists.
 *
 */
#include "List.h"
#include "game/addresses.h"

int List__remove_item_by_ptr(List* ECX, Entity* ptr)
{
    return ((List__remove_item_by_ptr_t)FUNC_LIST_REMOVE_ITEM_BY_DATA)(ECX, 0,
                                                                       ptr);
}
