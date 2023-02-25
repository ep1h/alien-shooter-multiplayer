/**
 * @file list.c
 * @brief Implementation of a doubly linked list.
 */
#include "list.h"
#include "utils/mem/mem.h"

typedef struct ListItem ListItem;

typedef struct ListItem
{
    void* data;
    ListItem* prev;
    ListItem* next;
} ListItem;

typedef struct List
{
    ListItem* front;
    ListItem* back;
    int items_number;
} List;

/**
 * @brief Removes an item from the list.
 *
 * @param[in] list Pointer to the list from which the item is removed.
 */
static void remove_item_(List* list, ListItem* item);

static void remove_item_(List* list, ListItem* item)
{
    if (!list || !list->items_number)
    {
        return;
    }
    if (item->prev)
    {
        item->prev->next = item->next;
    }
    else
    {
        list->front = item->next;
    }
    if (item->next)
    {
        item->next->prev = item->prev;
    }
    else
    {
        list->back = item->prev;
    }
    --list->items_number;
    mem_free(item);
}

List* list_create(void)
{
    List* list = mem_alloc(sizeof(List));
    if (!list)
    {
        return 0;
    }
    mem_set(list, 0, sizeof(List));
    return list;
}

void list_destroy(List* list)
{
    if (!list)
    {
        return;
    }
    list_clear(list);
    mem_free(list);
}

bool list_push_back(List* list, const void* data)
{
    if (!list)
    {
        return false;
    }
    ListItem* item = mem_alloc(sizeof(ListItem));
    mem_set(item, 0, sizeof(ListItem));
    if (!item)
    {
        return false;
    }

    if (list->items_number == 0)
    {
        list->front = item;
        list->back = item;
    }
    else
    {
        list->back->next = item;
        item->prev = list->back;
        list->back = item;
    }
    item->data = (void*)data;
    ++list->items_number;
    return true;
}

bool list_push_front(List* list, const void* data)
{
    if (!list)
    {
        return false;
    }
    ListItem* item = mem_alloc(sizeof(ListItem));
    mem_set(item, 0, sizeof(ListItem));
    if (!item)
    {
        return false;
    }

    if (list->items_number == 0)
    {
        list->front = item;
        list->back = item;
    }
    else
    {
        list->front->prev = item;
        item->next = list->front;
        list->front = item;
    }
    item->data = (void*)data;
    ++list->items_number;
    return true;
}

void* list_pop_back(List* list)
{
    if (!list || !list->items_number)
    {
        return 0;
    }
    ListItem* back_item = list->back;
    void* result = back_item->data;
    --list->items_number;
    if (list->items_number == 0)
    {
        list->back = 0;
        list->front = 0;
    }
    else
    {
        list->back = list->back->prev;
    }
    mem_free(back_item);
    return result;
}

void* list_pop_front(List* list)
{
    if (!list || !list->items_number)
    {
        return 0;
    }
    ListItem* front_item = list->front;
    void* result = front_item->data;
    --list->items_number;
    if (list->items_number == 0)
    {
        list->back = 0;
        list->front = 0;
    }
    else
    {
        list->front = list->front->next;
    }
    mem_free(front_item);
    return result;
}

void list_remove_by_data(List* list, void* data)
{
    if (!list || !list->items_number)
    {
        return;
    }
    ListItem* item = list->front;
    do
    {
        if (item->data == data)
        {
            remove_item_(list, item);
        }
        item = item->next;
    } while (item != list->back);
}

void list_clear(List* list)
{
    if (!list || !list->items_number)
    {
        return;
    }
    ListItem* i = list->front;
    while (i)
    {
        ListItem* tmp = i->next;
        mem_free(i);
        i = tmp;
    }
    list->back = 0;
    list->front = 0;
    list->items_number = 0;
}

int list_get_items_number(List* list)
{
    if (!list)
    {
        return 0;
    }
    return list->items_number;
}
