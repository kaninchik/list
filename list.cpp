#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "list.h"

const int INITAL_SIZE = 13;
const int POISON = 666;
const int FREE_CELL = -1;

int List_ctor(List *lst)
{
    lst->node = (Node *)calloc(INITAL_SIZE, sizeof(Node));
    lst->free = 1;
    lst->size = 1;
    lst->capacity = INITAL_SIZE;

    for(int i = 1; i < lst->capacity; i++)
    {
        lst->node[i].next = i + 1;
        lst->node[i].prev = -1;
    }

    lst->node[0].data = POISON;
    lst->node[0].next = 0;
    lst->node[0].prev = 0;

    return lst->free;
}

int Push_front(List *lst, int elem)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    int next_free_cell = lst->node[lst->free].next;

    lst->node[lst->free].data = elem;
    lst->node[lst->free].next = lst->node[0].next;
    lst->node[lst->free].prev = 0;
    lst->node[lst->node[0].next].prev = lst->free;
    lst->node[0].next = lst->free;

    lst->free = next_free_cell;
    lst->size++;

    return lst->free;
}

int Push_back(List *lst, int elem)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    int next_free_cell = lst->node[lst->free].next;

    lst->node[lst->free].data = elem;
    lst->node[lst->free].next = 0;
    lst->node[lst->free].prev = lst->node[0].prev;
    lst->node[lst->node[0].prev].next = lst->free;
    lst->node[0].prev = lst->free;

    lst->free = next_free_cell;
    lst->size++;

    return lst->free;
}

int List_dump(List *lst)
{
    assert(lst != nullptr);

    FILE *fp = fopen("list_dump.txt", "w+");

    assert(fp != nullptr);

    fprintf(fp, "DATA:     ");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "    %d    ", lst->node[i].data);

    fprintf(fp, "\n");

    fprintf(fp, "NEXT:     ");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "     %d    ", lst->node[i].next);

    fprintf(fp, "\n");

    fprintf(fp, "PREV:     ");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "     %d    ", lst->node[i].prev);

    fprintf(fp, "\n");
    fprintf(fp, "free =   %d\n", lst->free);

    fclose(fp);
}

int Pop_front(List *lst)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    int tmp = lst->node[0].next;

    lst->node[lst->node[0].next].prev = -1;
    lst->node[lst->node[lst->node[0].next].next].prev = 0;
    lst->node[0].next = lst->node[lst->node[0].next].next;
    lst->node[tmp].next = -1;

    lst->free = tmp;
    lst->size--;

    return lst->free;
}

int Pop_back(List *lst)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    int tmp = lst->node[0].prev;

    lst->node[lst->node[0].prev].next = -1;
    lst->node[lst->node[lst->node[0].prev].prev].next = 0;
    lst->node[0].prev = lst->node[lst->node[0].prev].prev;
    lst->node[tmp].prev = -1;

    lst->free = tmp;
    lst->size--;


    return lst->free;
}

int Insert_after(List *lst, int elem, size_t val)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    int next_free_cell = lst->node[lst->free].next;

    lst->node[lst->free].data = elem;
    lst->node[lst->free].next = lst->node[val].next;
    lst->node[lst->free].prev = val;
    lst->node[lst->node[val].next].prev = lst->free;
    lst->node[val].next = lst->free;

    lst->free = next_free_cell;
    lst->size++;

    return lst->free;
}

int Insert_before(List *lst, int elem, size_t val)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    int next_free_cell = lst->node[lst->free].next;

    lst->node[lst->free].data = elem;
    lst->node[lst->free].next = val;
    lst->node[lst->free].prev = lst->node[val].prev;
    lst->node[lst->node[val].prev].next = lst->free;
    lst->node[val].prev = lst->free;

    lst->free = next_free_cell;
    lst->size++;

    return lst->free;
}

int Delete(List *lst, size_t val)
{
    assert(lst != nullptr);

    if(lst->capacity <= lst->size)
        Do_realloc(lst);

    lst->node[val].data = 0;
    lst->node[lst->node[val].prev].next = lst->node[val].next;
    lst->node[lst->node[val].next].prev = lst->node[val].prev;
    lst->node[val].next = lst->free;
    lst->node[val].prev = -1;

    lst->free = val;
    lst->size--;

    return lst->free;
}

void Do_realloc(List *lst)
{
    assert(lst != nullptr);

    lst->capacity = 2 * lst->capacity;

    lst->node = (Node *)realloc(lst->node, lst->capacity * sizeof(Node));

    for(int i = INITAL_SIZE; i < lst->capacity; i++)
    {
        lst->node[i].data = 0;
        lst->node[i].next = i + 1;
        lst->node[i].prev = -1;
    }
}











