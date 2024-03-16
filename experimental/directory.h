/*
▒▒ ISC License

▒▒ Copyright (c) 2024 Sugiton <git@sugiton.dev>

▒▒ Permission to use, copy, modify, and/or distribute this software for any
▒▒ purpose with or without fee is hereby granted, provided that the above
▒▒ copyright notice and this permission notice appear in all copies.

▒▒ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
▒▒ REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
▒▒ AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
▒▒ INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
▒▒ LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
▒▒ OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
▒▒ PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef EXPERIMENTAL_DIRECTORY_H
#define EXPERIMENTAL_DIRECTORY_H

#include <experimental/memory.h>

#define _POSIX_SOURCE
#include <dirent.h>
#include <string.h>

#define directory                                                               \
    DIR * scoped(directoryclose)

#define directoryentry                                                          \
    struct dirent *

#define directorynames(path)                                                    \
    directorynamescallback(directorycount(path))(path)

#define directorynamescallback(count)                                           \
    ({                                                                          \
    char *(*(*callback)(char*))[count] = function(char *(*)[count], (char *path)\
        {                                                                       \
        directory      handle = directoryopen(path);                            \
        directoryentry entry  = NULL;                                           \
                                                                                \
        char *(*names)[count] = allocate (*names);                              \
        size_t  max           = arraysize(*names);                              \
        size_t  i             = 0;                                              \
                                                                                \
        while ((entry = directoryread(handle)) and i < max)                     \
            {                                                                   \
            (*names)[i] = strdup(entry->d_name);                                \
            i ++;                                                               \
            }                                                                   \
        return names;                                                           \
        });                                                                     \
    callback;                                                                   \
    })

#define directorynamesfree(array)                                               \
    directorynamesfreecallback(array)(array)

#define directorynamesfreecallback(array)                                       \
    ({                                                                          \
    void (*callback)(typeof(array)) = function(void, (typeof(array) pointers)   \
        {                                                                       \
        if (pointers)                                                           \
            {                                                                   \
            size_t n = arraysize(*pointers);                                    \
            while (n --> 0)                                                     \
                free((*pointers)[n]);                                           \
            free(pointers);                                                     \
            }                                                                   \
        });                                                                     \
    callback;                                                                   \
    })

#define directoryread(directory)                                                \
    readdir(directory)

static inline void (directoryclose)(handler)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    DIR **handler;
    {
    if (handler)
        closedir(*handler), *handler = NULL;
    }

static inline DIR *(directoryopen)(path)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    char *path;
    {
    DIR *handler = opendir(path);
    assertion(handler, "[experimental] directory access failed");
    return handler;
    }

static inline size_t (directorycount)(path)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    char *path;
    {
    directory      handler = directoryopen(path);
    directoryentry entry   = NULL;
    size_t          size   = 0;

    while ((entry = directoryread(handler)))
        size ++;

    return size;
    }

#endif
