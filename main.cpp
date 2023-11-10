#include <cstdio>

#include "list.h"

const int CAPACITY = 10;

int main()
{
    List list = {};

    List_ctor(&list, CAPACITY);

    for(int i = 1; i < 4; i++)
        Push_back(&list, i);

    List_dump(&list);
    List_dtor(&list);
    return 0;
}
