/*
::  ## ISO C
::      Lists are giving responsability to the user to cast the data
::      into the appropriate type and to free the memory.

::  ## GNU C
::      Lists are scoped and typed.
*/

#include <experimental/list.h>

#include <stdio.h>

typedef struct
{
    int x;
    int y;
}
    Point;

$ListOf(Point) * (create_points)(void)
{
    $list points = $_list_new(Point);

    for (int i = 10; i --> 0;)
    {
        $auto point = $_(points, prepend);
        $(point).x = $(point).y = i;
    }

    return $_memory_move_raw(points);
}

int (main)(void)
{
    $list points = create_points();

    for ($auto next = $_(points, forward); next; next = $_(points, forward))
        printf("(%i, %i) ", $(next).x, $(next).y);

    printf("\n");

    return EXIT_SUCCESS;
}
