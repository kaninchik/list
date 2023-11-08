#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "list.h"

const int INITAL_SIZE = 13;
const int NO_CONNECTION = 123;
const int POISON = 666;

int List_ctor(List *list)
{
    list->node = (Node *)calloc(INITAL_SIZE + 1, sizeof(Node));
    list->free = 1;
    list->size = 1;
    list->capacity = INITAL_SIZE + 1;

    for(int i = 1; i < list->capacity - 1; i++)
    {
        list->node[i].next = i + 1;
        list->node[i].prev = -1;
    }

    list->node[0].data = POISON;
    list->node[0].next = 0;
    list->node[0].prev = 0;

    list->node[list->capacity - 1].next = NO_CONNECTION;
    list->node[list->capacity - 1].prev = -1;

    return list->free;
}

int Push_front(List *list, int elem)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[list->free].next;

    list->node[list->free].data = elem;
    list->node[list->free].next = list->node[0].next;
    list->node[list->free].prev = 0;

    list->node[list->node[0].next].prev = list->free;
    list->node[0].next = list->free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int Push_back(List *list, int elem)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[list->free].next;

    list->node[list->free].data = elem;
    list->node[list->free].next = 0;
    list->node[list->free].prev = list->node[0].prev;
    list->node[list->node[0].prev].next = list->free;
    list->node[0].prev = list->free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int List_dump(List *list)
{
    assert(list != nullptr);

    FILE *fp = fopen("dump.dot", "w+");

    if(fp == nullptr)
    {
        printf("I can't open the file, go get some sleep, honey) you'll continue in the morning...");
        abort();
    }

    fprintf(fp, "digraph G                                                                               \n"
                "{                                                                                       \n"
                "   rankdir = LR;                                                                        \n"
                "   node [style = \"filled\", shape = record, color = \"black\"];       \n"
                "   size [fillcolor = \"pink\", label = \"size = %d\"];                                  \n"
                "   capacity [fillcolor = \"violet\", label = \"capacity = %d\"];                        \n"
                "   free [fillcolor = \"purple\", label = \"free = %d\"];                                \n"
                "   tail [fillcolor = \"red\", label = \"tail = %d\"];                                   \n"
                "   head [fillcolor = \"red\", label = \"head = %d\"];                                   \n",
                list->size, list->capacity, list->free, list->node[0].next, list->node[0].prev);

    for(int i = 0; i < list->capacity; i++)
    {
         fprintf(fp,       "    node%d [fillcolor = \"purple\", label = \" %d | { <prev> prev = %d |data = %d| <next> next = %d}\"];      \n",
                            i, i, list->node[i].prev, list->node[i].data, list->node[i].next);
    }

    fprintf(fp, "   edge [style = invis, constraint = true];\n");
    for(int i = 0; i < list->capacity - 1; i++)
        fprintf(fp, "    node%d: <next> -> node%d: <next>;\n", i, i + 1);

    fprintf(fp, "   edge [color = \"green\", constraint = true, style = solid];\n");
    for(int i = 0; i < list->capacity - 1; i++)
        fprintf(fp, "    node%d: <next> -> node%d: <next>;\n", i, list->node[i].next);

    fprintf(fp, "   edge [color = \"blue\", constraint = true, style = solid];\n");
    for(int i = 0; i < list->capacity - 1; i++)
    {
        if(list->node[i].prev != - 1)
            fprintf(fp, "    node%d: <prev> -> node%d: <prev>;\n", i, list->node[i].prev);
    }

    fprintf(fp, "}\n");

    fclose(fp);
}

int Pop_front(List *list, int *elem)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int tmp = list->node[0].next;
    *elem = list->node[list->node[0].next].data;

    list->node[list->node[0].next].prev = -1;
    list->node[list->node[list->node[0].next].next].prev = 0;
    list->node[0].next = list->node[list->node[0].next].next;
    list->node[tmp].next = list->free;

    list->free = tmp;
    list->size--;

    return tmp;
}

int Pop_back(List *list, int *elem)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int tmp = list->node[0].prev;
    *elem = list->node[list->node[0].prev].data;

    list->node[list->node[0].prev].next = list->free;
    list->node[list->node[list->node[0].prev].prev].next = 0;
    list->node[0].prev = list->node[list->node[0].prev].prev;
    list->node[tmp].prev = -1;

    list->free = tmp;
    list->size--;

    return tmp;
}

int Insert_after(List *list, int elem, size_t val)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[list->free].next;

    list->node[list->free].data = elem;
    list->node[list->free].next = list->node[val].next;
    list->node[list->free].prev = val;
    list->node[list->node[val].next].prev = list->free;
    list->node[val].next = list->free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int Insert_before(List *list, int elem, size_t val)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[list->free].next;

    list->node[list->free].data = elem;
    list->node[list->free].next = val;
    list->node[list->free].prev = list->node[val].prev;
    list->node[list->node[val].prev].next = list->free;
    list->node[val].prev = list->free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int Delete(List *list, size_t val, int *elem)
{
    assert(list != nullptr);

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    *elem = list->node[val].data;

    list->node[val].data = 0;
    list->node[list->node[val].prev].next = list->node[val].next;
    list->node[list->node[val].next].prev = list->node[val].prev;
    list->node[val].next = list->free;
    list->node[val].prev = -1;

    list->free = val;
    list->size--;

    return val;
}

void Do_realloc(List *list)
{
    assert(list != nullptr);

    list->capacity = 2 * list->capacity;

    printf("capacity = %d", list->capacity);

    list->node = (Node *)realloc(list->node, list->capacity * sizeof(Node));

    for(int i = list->size; i < list->capacity; i++)
    {
        list->node[i].data = 0;
        list->node[i].next = i + 1;
        list->node[i].prev = -1;
    }
}

void List_verificator(List *list)
{
    if(list == nullptr)
    {
        printf("The pointer to the list structure is null! Goodbye, my darling)");
        abort();
    }

    if(list->node == nullptr)
    {
        printf("A pointer to an array with nodes is null. Your program says goodbye to you...");
        abort();
    }

    if(list->capacity < list->size)
    {
        printf("The capacity exceeds the size, be careful, my beloved");
        abort();
    }

    if(list->node[0].data != POISON)
    {
        printf("Your actions have changed the fictitious element. I didn't know you were such a bully");
        abort();
    }
}

void List_dtor(List *list)
{
    free(list->node);
    list->free = -1;
    list->size = -1;
    list->capacity = -1;
}
