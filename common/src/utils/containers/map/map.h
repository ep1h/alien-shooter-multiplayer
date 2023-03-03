#ifndef UTILS_CONTAINERS_MAP_MAP_H
#define UTILS_CONTAINERS_MAP_MAP_H

#include <stdbool.h>

typedef struct Map Map;
typedef void (*Map_foreach_callback)(int key, void* value);

Map* map_create(void);
void map_destroy(Map* map);
void map_clear(Map* map);
bool map_insert(Map* map, int key, void* data);
bool map_erase(Map* map, int key);
void* map_find(Map* map, int key);
void map_foreach(Map* map, Map_foreach_callback callback);

#endif /* UTILS_CONTAINERS_MAP_MAP_H */
