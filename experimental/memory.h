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

#ifndef EXPERIMENTAL_MEMORY_H
#define EXPERIMENTAL_MEMORY_H

#include <assert.h>
#include <iso646.h>
#include <stdlib.h>

#if not defined(__GNUC__)                                                       \
    or  defined(__clang__)                                                      \
    or  __GNUC__ <  4                                                           \
    or (__GNUC__ == 4 and __GNUC_MINOR__ < 9)
#   include <experimental requires gnu c dialect, gcc compiler 4.9 or above>
#endif

//──────────────────────────────────────────────────────────────────────────────

#define allocate(type)         ((typeof(type)*)(allocate)(sizeof(type)))
#define arraylast(array)       (arraysize(array)-1)
#define autofree               autotype scoped(destroy)
#define autotype               __auto_type
#define function(type, body)   ({typeof(type) _ body _;})
#define move(pointer)          ((typeof(pointer))(move)(&pointer))
#define moveraw(pointer)       ((move)(&pointer))
#define scoped(callback)       __attribute__((cleanup(callback)))

#define arraysize(array)                                                        \
    ({                                                                          \
    static_assert                                                               \
        (not __builtin_types_compatible_p                                       \
                (typeof(array), typeof(&array[0]))                              \
        , "[experimental] requires static or variable length array"             \
        );                                                                      \
    (sizeof(array) / sizeof(array[0]));                                         \
    })

#define assertion(condition, message)                                           \
    ({                                                                          \
    static_assert                                                               \
        (   __builtin_constant_p                                                \
                (message)                                                       \
        and __builtin_types_compatible_p                                        \
                (typeof(message), typeof(char[sizeof(message)]))                \
        , "[experimental] requires string literal message"                      \
        );                                                                      \
    (not (condition) ? assert(not message) : (void)0);                          \
    })

//──────────────────────────────────────────────────────────────────────────────

static inline void *(allocate)(size)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    size_t size;
    {
    void *space = calloc(1, size);
    assertion(space, "[experimental] memory allocation failed");
    return space;
    }

static inline void *(move)(pointer)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    void *pointer;
    {
    void **giver = pointer;
    void  *taker = NULL;
    if (giver)
        taker = *giver, *giver = NULL;
    return taker;
    }

static inline void (destroy)(pointer)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    void *pointer;
    {
    void **space = pointer;
    if (space)
        free(*space), *space = NULL;
    }

#endif
