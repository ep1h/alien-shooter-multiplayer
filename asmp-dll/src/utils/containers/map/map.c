/**
 * @file map.c
 * @brief Implementation of a generic map data structure and its associated
 *        functions.
 */
#include "map.h"
#include <stdlib.h>
#include "utils/mem/mem.h"

typedef struct MapItem
{
    int key;
    void* data;
} MapItem;

typedef struct Map
{
    MapItem* data;
    int items_number;
    int capacity;
} Map;


/**
 * @brief Compares two MapItem instances.
 *
 * @param[in] a Pointer to the first MapItem instance.
 * @param[in] b Pointer to the second MapItem instance.
 *
 * @return int 0 if the keys are equal
 * @return int > 0 if the key of the first MapItem is greater than the key of
 *         the second MapItem
 * @return int < 0 if the key of the first MapItem is less than the key of the
 *         second MapItem
 */
static int srot_func_(const void* a, const void* b);

/**
 * @brief Searches for a MapItem instance with the specified key.
 *
 * @param[in] key The key to be searched.
 * @param[in] b   Pointer to the MapItem instance.
 *
 * @return int 0 if the keys are equal
 * @return int > 0 if the key is greater than the key of the MapItem instance
 * @return int < 0 if the key is less than the key of the MapItem instance
 */
static int search_func_(const void* key, const void* b);

/**
 * @brief Searches for a MapItem instance with the specified key.
 *
 * @param[in] map Pointer to the map in which the MapItem instance is searched.
 * @param[in] key The key to be searched.
 *
 * @return MapItem* Pointer to the MapItem instance if found.
 * @return NULL If not found.
 */
static MapItem* find_item_(Map* map, int key);


static int srot_func_(const void* a, const void* b)
{
    return (((const MapItem*)a)->key - ((const MapItem*)b)->key);
}

static int search_func_(const void* key, const void* b)
{
    return ((int)key - (int)((const MapItem*)b)->key);
}

static MapItem* find_item_(Map* map, int key)
{
    if (!map || !map->data)
    {
        return 0;
    }
    return bsearch((const void*)key, map->data, map->items_number,
                   sizeof(MapItem), search_func_);
}

Map* map_create(void)
{
    Map* map = mem_alloc(sizeof(Map));
    if (!map)
    {
        return 0;
    }
    mem_set(map, 0, sizeof(Map));
    return map;
}

void map_destroy(Map* map)
{
    if (!map)
    {
        return;
    }
    map_clear(map);
    mem_free(map);
}

void map_clear(Map* map)
{
    if (!map)
    {
        return;
    }
    map->capacity = 0;
    map->items_number = 0;
    if (!map->data)
    {
        return;
    }
    mem_free(map->data);
    map->data = 0;
}

bool map_insert(Map* map, int key, void* data)
{
    if (!map)
    {
        return false;
    }
    if (map_find(map, key))
    {
        return false;
    }
    if (map->items_number >= map->capacity)
    {
        int new_capacity = 0;
        if (map->capacity == 0)
        {
            new_capacity = 1;
        }
        else
        {
            new_capacity = map->capacity * 2;
        }
        void* new_data = mem_realloc(map->data, new_capacity * sizeof(MapItem));
        if (new_data)
        {
            map->capacity = new_capacity;
            map->data = new_data;
        }
        else
        {
            return false;
        }
    }
    map->data[map->items_number].key = key;
    map->data[map->items_number].data = data;
    ++map->items_number;
    qsort(map->data, /*map->capacity*/ map->items_number, sizeof(MapItem),
          srot_func_);
    return true;
}

bool map_erase(Map* map, int key)
{
    if (!map || !map->data)
    {
        return false;
    }
    MapItem* item = find_item_(map, key);
    if (!item)
    {
        return false;
    }
    item->key = map->data[map->items_number - 1].key;
    item->data = map->data[map->items_number - 1].data;
    mem_set(&map->data[map->items_number - 1], 0, sizeof(MapItem));
    --map->items_number;
    qsort(map->data, /*map->capacity*/ map->items_number, sizeof(MapItem),
          srot_func_);
    // TODO: Adjust capacity
    return true;
}

void* map_find(Map* map, int key)
{
    if (!map)
    {
        return 0;
    }
    // TODO: Check is map->data allocated
    MapItem* map_item = bsearch((const void*)key, map->data, map->items_number,
                                sizeof(MapItem), search_func_);
    if (map_item)
    {
        return map_item->data;
    }
    return 0;
}

void map_foreach(Map* map, Map_foreach_callback callback)
{
    if (!map || !map->items_number || !callback)
    {
        return;
    }
    for (int i = 0; i < map->items_number; i++)
    {
        callback(map->data[i].key, map->data[i].data);
    }
}
