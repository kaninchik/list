#include <cstdio>

#include "list.h"

int main()
{
    List lst = {};

    List_ctor(&lst);

    for(int i = 1; i < 5; i++)
        Push_back(&lst, i);

    Delete(&lst, 3);

    Insert_before(&lst, 12, 2);
    Insert_after(&lst, 25, 3);

    List_dump(&lst);
    return 0;
}

