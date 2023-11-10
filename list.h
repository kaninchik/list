#ifndef LIST_H
#define LIST_H

typedef int list_t;

struct Node
{
    list_t data = 0;
    list_t next = 0;
    list_t prev = 0;
};

struct List
{
    Node *node = nullptr;
    int capacity = 0;
    int size = 0;
    int free = 0;
};

int List_ctor(List *list, int capacity);
int Push_front(List *list, list_t elem);
int Find_phys_index(List *list, size_t logical_index);
int Insert_after(List *list, size_t index, list_t elem);
int Insert_before(List *list, size_t index, list_t elem);
int Delete(List *list, size_t index, list_t *elem);
int Push_back(List *list, list_t elem);
int List_dump(List *list);
int Pop_front(List *list, list_t *elem);
int Pop_back(List *list, list_t *elem);
int List_verificator(List *list);
int Do_realloc(List *list);
int Fill_free_cells(List *list);
void List_dtor(List *list);


#endif
