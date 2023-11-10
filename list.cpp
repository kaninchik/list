#include <cstdio>
#include <cassert>
#include <cstdlib>

#include "list.h"

const int LAST_CONNECTION = 123;
const int POISON = 666;
const int LIST_FAULT = 0;
const int lIST_IN_ORDER = 1;

int List_ctor(List *list, int capacity)
{
    if(capacity <= 0)
    {
        printf("Think about the value of the initial capacity again\n");
        return LIST_FAULT;
    }

    list->node = (Node *)calloc(capacity + 1, sizeof(Node));

    if(list->node == nullptr)
    {
        printf("There is news: the calloc hasn't worked\n");
        return LIST_FAULT;
    }

    list->free = 1;
    list->size = 1;
    list->capacity = capacity + 1;

    list->node[0].data = POISON;
    list->node[0].next = 0;
    list->node[0].prev = 0;

    Fill_free_cells(list);

    return list->free;
}

int Push_front(List *list, list_t elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[list->free].next;

    list->node[free].data = elem;
    list->node[free].next = list->node[0].next;
    list->node[free].prev = 0;

    list->node[list->node[0].next].prev = free;
    list->node[0].next = free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int Push_back(List *list, list_t elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[free].next;

    list->node[free].data = elem;
    list->node[free].next = 0;
    list->node[free].prev = list->node[0].prev;
    list->node[list->node[0].prev].next = free;
    list->node[0].prev = free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int List_dump(List *list)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    FILE *fp = fopen("dump.dot", "w+");

    if(fp == nullptr)
    {
        printf("I can't open the file, go get some sleep, honey) you'll continue in the morning...\n");
        return LIST_FAULT;
    }

    fprintf(fp, "digraph G                                                            \n"
                "{                                                                    \n"
                "   graph [bgcolor = \"#ffcbdb\"]                                     \n"
                "   rankdir = LR;                                                     \n"
                "   node [style = \"filled\", shape = record, color = \"black\"];     \n");

    for(int i = 0; i < list->capacity; i++)
    {
        if(list->node[i].prev != -1)
            fprintf(fp,     "    node%d [fillcolor = \"purple\", label = \" %d |"
                            "{ <prev> prev = %d |data = %d| <next> next = %d}\"];\n",
                        i, i, list->node[i].prev, list->node[i].data, list->node[i].next);
        else
            fprintf(fp,     "    node%d [fillcolor = \"#f64a8a\", label = \" %d |"
                            "{ <prev> prev = %d |data = %d| <next> next = %d}\"];\n",
                        i, i, list->node[i].prev, list->node[i].data, list->node[i].next);
    }

    fprintf(fp, "   edge [style = invis];\n");
    for(int i = 0; i < list->capacity - 1; i++)
        fprintf(fp, "    node%d -> node%d;\n", i, i + 1);

    fprintf(fp, "   edge [color = \"black\", style = solid, weight = 0];\n");
    for(int i = 0; i < list->capacity - 1; i++)
        fprintf(fp, "    node%d -> node%d;\n", i, list->node[i].next);

    /*fprintf(fp, "   edge [color = \"blue\", constraint = false, style = solid, weight = 0];\n");
    for(int i = 0; i < list->capacity - 1; i++)
    {
        if(list->node[i].prev != - 1)
            fprintf(fp, "    node%d: <prev> -> node%d;\n", i, list->node[i].prev);
    }
    */

    fprintf(fp, "   size [fillcolor = \"pink\", label = \"size = %d\"];               \n"
                "   capacity [fillcolor = \"violet\", label = \"capacity = %d\"];     \n"
                "   free [fillcolor = \"purple\", label = \"free = %d\"];             \n"
                "   tail [fillcolor = \"red\", label = \"tail = %d\"];                \n"
                "   head [fillcolor = \"red\", label = \"head = %d\"];                \n",
                list->size, list->capacity, list->free, list->node[0].prev, list->node[0].next);

    fprintf(fp, "   edge [color = \"#ab274f\", style = bold, constraint = false];           \n");
    fprintf(fp, "   free -> node%d;                                                          \n"
                "   tail  -> node%d;                                                         \n"
                "   head  -> node%d;                                                         \n",
                list->free, list->node[0].prev, list->node[0].next);

    fprintf(fp, "}\n");

    fclose(fp);
}

int Pop_front(List *list, list_t *elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int index = list->node[0].next;
    *elem = list->node[index].data;

    list->node[index].prev = -1;
    list->node[list->node[index].next].prev = 0;
    list->node[0].next = list->node[index].next;
    list->node[index].next = list->free;

    list->free = index;
    list->size--;

    return index;
}

int Pop_back(List *list, list_t *elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int index = list->node[0].prev;
    *elem = list->node[list->node[0].prev].data;

    list->node[index].next = list->free;
    list->node[list->node[index].prev].next = 0;
    list->node[0].prev = list->node[index].prev;
    list->node[index].prev = -1;

    list->free = index;
    list->size--;

    return index;
}

int Insert_after(List *list, size_t index, list_t elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[free].next;

    list->node[free].data = elem;
    list->node[free].next = list->node[index].next;
    list->node[free].prev = index;
    list->node[list->node[index].next].prev = free;
    list->node[index].next = free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int Insert_before(List *list, size_t index, list_t elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    int free = list->free;
    int next_free_cell = list->node[free].next;

    list->node[free].data = elem;
    list->node[free].next = index;
    list->node[free].prev = list->node[index].prev;
    list->node[list->node[index].prev].next = free;
    list->node[index].prev = free;

    list->free = next_free_cell;
    list->size++;

    return free;
}

int Delete(List *list, size_t index, list_t *elem)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    if(list->capacity <= list->size + 1)
        Do_realloc(list);

    *elem = list->node[index].data;

    list->node[index].data = 0;
    list->node[list->node[index].prev].next = list->node[index].next;
    list->node[list->node[index].next].prev = list->node[index].prev;
    list->node[index].next = list->free;
    list->node[index].prev = -1;

    list->free = index;
    list->size--;

    return index;
}

int Do_realloc(List *list)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    list->capacity = 2 * list->capacity;

    list->node = (Node *)realloc(list->node, list->capacity * sizeof(Node));

    if(list->node == nullptr)
    {
        printf("There is news: the realloc hasn't worked\n");
        return LIST_FAULT;
    }

    Fill_free_cells(list);
}

int List_verificator(List *list)
{
    if(list == nullptr)
    {
        printf("The pointer to the list structure is null! Goodbye, my darling)\n");
        return LIST_FAULT;
    }

    if(list->node == nullptr)
    {
        printf("A pointer to an array with nodes is null. Your program says goodbye to you...\n");
        return LIST_FAULT;
    }

    if(list->capacity < list->size)
    {
        printf("The capacity exceeds the size, be careful, my beloved\n");
        return LIST_FAULT;
    }

    if(list->capacity < 0)
    {
        printf("The capacity less zero, be careful, my beloved\n");
        return LIST_FAULT;
    }

    if(list->size < 0)
    {
        printf("The size less zero, be careful, my beloved\n");
        return LIST_FAULT;
    }

    if(list->node[0].data != POISON)
    {
        printf("Your actions have changed the fictitious element. I didn't know you were such a bully\n");
        return LIST_FAULT;
    }

    return lIST_IN_ORDER;
}

int Fill_free_cells(List *list)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    for(int i = list->size; i < list->capacity - 1; i++)
    {
        list->node[i].next = i + 1;
        list->node[i].prev = -1;
    }

    list->node[list->capacity - 1].next = LAST_CONNECTION;
    list->node[list->capacity - 1].prev = -1;
}

int Find_phys_index(List *list, size_t logical_index)
{
    if(!List_verificator(list))
        return LIST_FAULT;

    size_t phys_index = list->node[0].next;

    for(int i = 1; i < logical_index; i++)
        phys_index = list->node[phys_index].next;

    return phys_index;
}

void List_dtor(List *list)
{
    free(list->node);
    list->free = -1;
    list->size = -1;
    list->capacity = -1;
}
