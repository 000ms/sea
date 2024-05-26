/*─────────────────────────────────────────────────────────────────────────────┐
 ISO C
   Lists are giving responsability to the user to cast the data
   into the appropriate type and to free the memory.

 GNU C
   Lists are scoped and typed.
└─────────────────────────────────────────────────────────────────────────────*/

#include <sea/list.h>
#include <stdio.h>

/*─────────────────────────────────────────────────────────────────────────────┐
 types
    Point

 functions
    createpoints () : creates a list of ten points
                    , initialized with their respective indexes
                    , (0, 0) <-> (1, 1) ... (8, 8) <-> (9, 9)

 main
    sets points = createpoints ()
    prints points
└─────────────────────────────────────────────────────────────────────────────*/

typedef struct
{
    int x;
    int y;
}
    Point;

//──────────────────────────────────────────────────────────────────────────────

pointer (ListOf (Point)) (createpoints) (void)
{
    list points = allocatelistof (Point);

    for (int i = 10; i --> 0;)
    {
        inferred point = self (points, prepend);
        (*point).data.x = (*point).data.y = i;
    }

    return move (points);
}

//──────────────────────────────────────────────────────────────────────────────

int (main) (void)
{
    list points = createpoints ();

    for (inferred next = self (points, forward);
                  next;
                  next = self (points, forward))
        printf ("(%i, %i) ", (*next).data.x, (*next).data.y);

    printf ("\n");

    return EXIT_SUCCESS;
}
