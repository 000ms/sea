/*──────────────────────────────────────────────────────────────────────────────┐
│ ISO C                                                                         │
│   Lists are giving responsability to the user to cast the data                │
│   into the appropriate type and to free the memory.                           │
│                                                                               │
│ GNU C                                                                         │
│   Lists are scoped and typed.                                                 │
└──────────────────────────────────────────────────────────────────────────────*/

#include <sea/list.h>
#include <stdio.h>

/*──────────────────────────────────────────────────────────────────────────────┐
│                             a two dimension point                             │
└──────────────────────────────────────────────────────────────────────────────*/

typedef struct
{
    int x;
    int y;
}
    Point;

/*──────────────────────────────────────────────────────────────────────────────┐
│    creates a list of ten points initialized with their respective indexes     │
│                    (0, 0) <-> (1, 1) ... (8, 8) <-> (9, 9)                    │
└──────────────────────────────────────────────────────────────────────────────*/

pointer (ListOf (Point)) (createpoints) (void)
{
    list points = allocatelistof (Point);

    for (int i = 10; i --> 0;)
    {
        inferred point = invoke (points, prepend);
        (*point).data.x = (*point).data.y = i;
    }

    return move (points);
}

/*──────────────────────────────────────────────────────────────────────────────┐
│                   displays the list of ten points in order                    │
└──────────────────────────────────────────────────────────────────────────────*/

int (main) (void)
{
    list points = createpoints ();

    for (inferred next = invoke (points, forward);
                  next;
                  next = invoke (points, forward))
        printf ("(%i, %i) ", (*next).data.x, (*next).data.y);

    printf ("\n");

    return EXIT_SUCCESS;
}
