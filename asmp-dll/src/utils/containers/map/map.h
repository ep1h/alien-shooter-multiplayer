/**
 * @file map.h
 * @brief Defines a generic map data structure and its associated functions.
 */
#ifndef UTILS_CONTAINERS_MAP_MAP_H
#define UTILS_CONTAINERS_MAP_MAP_H

#include <stdbool.h>

typedef struct Map Map;
typedef void (*Map_foreach_callback)(int key, void* value);

/**
 * @brief Creates and initializes a new map instance.
 *
 * Allocates memory for a new map and initializes it.
 *
 * @return Map* A pointer to the created map instance.
 * @return NULL If failed.
 */
Map* map_create(void);

/**
 * @brief Destroys a map instance and deallocates its memory.
 *
 * Frees the memory allocated for the map and its elements.
 *
 * @param[in] map Pointer to the map to be destroyed.
 */
void map_destroy(Map* map);

/**
 * @brief Clears all key-value pairs from the map.
 *
 * @param[in] map Pointer to the map to be cleared.
 */
void map_clear(Map* map);

/**
 * @brief Inserts a new key-value pair into the map.
 *
 * @param[in] map Pointer to the map in which the key-value pair is inserted.
 * @param[in] key The key associated with the data.
 * @param[in] data Pointer to the data to be inserted.
 *
 * @return true if the insertion is successful
 * @return false if an error occurs.
 */
bool map_insert(Map* map, int key, void* data);

/**
 * @brief Removes a key-value pair from the map based on the specified key.
 *
 * @param[in] map Pointer to the map from which the key-value pair is erased.
 * @param[in] key The key associated with the data to be erased.
 *
 * @return true if the erasure is successful.
 * @return false if the key is not found.
 */
bool map_erase(Map* map, int key);

/**
 * @brief Searches for the data associated with a given key in the map.
 *
 * @param[in] map Pointer to the map in which the data is searched.
 * @param[in] key The key for which data is searched.
 *
 * @return A pointer to the data associated with the key
 * @return NULL if the key is not found.
 */
void* map_find(Map* map, int key);

/**
 * @brief Iterates through the map, applying a specified callback function to
 *        each key-value pair.
 *
 * @param[in] map Pointer to the map to be iterated.
 * @param[in] callback Callback function to be applied to each key-value pair.
 */
void map_foreach(Map* map, Map_foreach_callback callback);

#endif /* UTILS_CONTAINERS_MAP_MAP_H */
