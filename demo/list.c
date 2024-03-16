/*
::  ## ISO C
::      Lists are implemented with data as void pointer
::      giving responsability to the user to cast it
::      into the appropriate type

::  ## GNU C
::      Using generics the data field is typed
*/

#include <experimental/list.h>

#include <stdio.h>

typedef struct
    {
    int x;
    int y;
    }
    Point;

listof(Point) *(createpoints)(void)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    {
    autolist points = allocatelistof(Point);

    for (int i = 10; i --> 0;)
        {
        autotype p = listprepend(points);
        p->data.x = p->data.y = i;
        }

    return moveraw(points);
    }

int (main)(void)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    {
    autolist points = createpoints();

    for (autotype next = listforward(points); next; next = listforward(points))
        printf("(%i, %i) ", next->data.x, next->data.y);

    printf("\n");

    return EXIT_SUCCESS;
    }
