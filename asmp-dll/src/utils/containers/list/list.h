#ifndef UTILS_CONTAINERS_LIST_LIST_H
#define UTILS_CONTAINERS_LIST_LIST_H

#include <stdbool.h>

typedef struct List List;

List* list_create(void);
void list_destroy(List* list);
bool list_push_back(List* list, const void* data);
bool list_push_front(List* list, const void* data);
void* list_pop_back(List* list);
void* list_pop_front(List* list);
void list_remove_by_data(List* list, void* data);
void list_clear(List* list);
int list_get_items_number(List* list);

#endif /* UTILS_CONTAINERS_LIST_LIST_H */
