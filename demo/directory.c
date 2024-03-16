/*
::  ##  Reads a directory
::      Builds an array with the element's names

::  __  [note] The built array length equals the count of found elements

::  __  [note] This demonstrates how to return a VLA
::             using generics with a scalar parameter
*/

#include <experimental/directory.h>

#include <stdio.h>

int (main)(void)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    {
    autotype tab = directorynames(".");

    for (size_t i = 0; i < arraysize(*tab); i ++)
        printf("%s\n", (*tab)[i]);

    directorynamesfree(tab);

    return EXIT_SUCCESS;
    }
