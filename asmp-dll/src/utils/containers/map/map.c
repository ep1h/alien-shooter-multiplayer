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

static int srot_func(const void* a, const void* b);
static int search_func(const void* key, const void* b);
static MapItem* find_item(Map* map, int key);

static int srot_func(const void* a, const void* b)
{
    return (((const MapItem*)a)->key - ((const MapItem*)b)->key);
}

static int search_func(const void* key, const void* b)
{
    return ((int)key - (int)((const MapItem*)b)->key);
}

static MapItem* find_item(Map* map, int key)
{
    if (!map || !map->data)
    {
        return 0;
    }
    return bsearch((const void*)key, map->data, map->items_number,
                   sizeof(MapItem), search_func);
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
          srot_func);
    return true;
}

bool map_erase(Map* map, int key)
{
    if (!map || !map->data)
    {
        return false;
    }
    MapItem* item = find_item(map, key);
    if (!item)
    {
        return false;
    }
    item->key = map->data[map->items_number - 1].key;
    item->data = map->data[map->items_number - 1].data;
    mem_set(&map->data[map->items_number - 1], 0, sizeof(MapItem));
    --map->items_number;
    qsort(map->data, /*map->capacity*/ map->items_number, sizeof(MapItem),
          srot_func);
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
                                sizeof(MapItem), search_func);
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
