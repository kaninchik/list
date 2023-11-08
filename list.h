#ifndef LIST_H
#define LIST_H

struct Node
{
    int data = 0;
    int next = 0;
    int prev = 0;
};

struct List
{
    Node *node = nullptr;
    int capacity = 0;
    int size = 0;
    int free = 0;
};

int List_ctor(List *list);
int Push_front(List *list, int elem);
int Insert_after(List *list, int elem, size_t val);
int Insert_before(List *list, int elem, size_t val);
int Delete(List *list, size_t val, int *elem);
int Push_back(List *list, int elem);
int List_dump(List *list);
int Pop_front(List *list, int *elem);
int Pop_back(List *list, int *elem);
void Do_realloc(List *list);
void List_dtor(List *list);
void List_verificator(List *list);


#endif
