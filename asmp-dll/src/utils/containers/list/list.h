#ifndef UTILS_CONTAINERS_LIST_LIST_H
#define UTILS_CONTAINERS_LIST_LIST_H

#include <stdbool.h>

typedef struct List List;

/**
 * @brief Creates a list instance.
 *
 * @return List* pointer to created list instance.
 * @return NULL if failed.
 */
List* list_create(void);

/**
 * @brief Destroys a list instance.
 *
 * Frees memory allocated for the list and its items.
 *
 * @param[in] list Pointer to the list to be destroyed.
 */
void list_destroy(List* list);

/**
 * @brief Adds an item to the end of the list.
 *
 * @param[in] list Pointer to the list to which the item is added.
 * @param[in] data Pointer to the data to be added to the list.
 *
 * @return true If the item is successfully added.
 * @return false If an error occurs.
 */
bool list_push_back(List* list, const void* data);

/**
 * @brief Adds an item to the beginning of the list.
 *
 * @param[in] list Pointer to the list to which the item is added.
 * @param[in] data Pointer to the data to be added to the list.
 *
 * @return true If the item is successfully added.
 * @return false If an error occurs.
 */
bool list_push_front(List* list, const void* data);

/**
 * @brief Removes the last list item and returns it's data.
 *
 * @param[in] list Pointer to the list from which the item is removed.
 *
 * @return A pointer to the removed item's data.
 * @return NULL if the list is empty or if an error occurs.
 */
void* list_pop_back(List* list);

/**
 * @brief Removes the first list item and returns it's data.
 *
 * @param[in] list Pointer to the list from which the item is removed.
 *
 * @return A pointer to the removed item's data or NULL if the list is empty.
 * @return NULL if the list is empty or if an error occurs.
 */
void* list_pop_front(List* list);

/**
 * @brief Removes all occurrences of an item with the specified data from the
 *        list.
 *
 * @param[in] list Pointer to the list from which items are removed.
 * @param[in] data Pointer to the data to be removed from the list.
 */
void list_remove_by_data(List* list, void* data);

/**
 * @brief Clears the entire list, removing all items.
 *
 * @param[in] list Pointer to the list to be cleared.
 */
void list_clear(List* list);

/**
 * @brief Returns the number of list items.
 *
 * @param[in] list Pointer to the list for which the item count is determined.
 *
 * @return The number of list items.
 */
int list_get_items_number(List* list);

#endif /* UTILS_CONTAINERS_LIST_LIST_H */
