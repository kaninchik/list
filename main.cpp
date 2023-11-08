#include <cstdio>

#include "list.h"

int main()
{
    List list = {};

    List_ctor(&list);

    for(int i = 1; i < 4; i++)
        Push_back(&list, i);

    Insert_after(&list, 55, 1);

    Insert_before(&list, 22, 3);

    Push_front(&list, 34);

    Push_back(&list, 22);

    int a = 0;

    Pop_front(&list, &a);

    List_dump(&list);
    List_dtor(&list);

    return 0;
}
