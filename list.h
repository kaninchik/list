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
    Node *node = NULL;
    int capacity = 0;
    int size = 0;
    int free = 0;
};

int List_ctor(List *lst);
int Push_front(List *lst, int elem);
int Insert_after(List *lst, int elem, size_t val);
int Insert_before(List *lst, int elem, size_t val);
int Delete(List *lst, size_t val);
int Push_back(List *lst, int elem);
int List_dump(List *lst);
int Pop_front(List *lst);
int Pop_back(List *lst);
void Do_realloc(List *lst);


#endif
