/*─────────────────────────────────────────────────────────────────────────────┐
 ISC License

 Copyright (c) 2024 サイトー <https://zero.ms>

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 PERFORMANCE OF THIS SOFTWARE.
└─────────────────────────────────────────────────────────────────────────────*/

#pragma once

#include <sea/syntax.h>
#include <stdlib.h>

/*─────────────────────────────────────────────────────────────────────────────┐
 destroyed : inferred scoped-bound contiguous dynamic memory handler
└─────────────────────────────────────────────────────────────────────────────*/

#define destroyed                                                              \
    scoped (destroy) inferred

/*─────────────────────────────────────────────────────────────────────────────┐
 allocate (type)    : allocates sizeof (type) zero initialized memory
 move     (pointer) : returns typeless pointer swapped ownership
 movetype (pointer) : returns typesafe pointer swapped ownership
 destroy  (pointer) : free (pointer) with dangling address erasure
└─────────────────────────────────────────────────────────────────────────────*/

#define allocate(_type_)                                                       \
    ((pointer (_type_))allocate (sizeof (_type_)))

#define move(_pointer_)                                                        \
    (move (&_pointer_))

#define movetype(_pointer_)                                                    \
    ((typeof (_pointer_))move (_pointer_))

#define destroy(_pointer_)                                                     \
    (destroy (&_pointer_))

/*─────────────────────────────────────────────────────────────────────────────┐
 allocate (size) : allocates (size) bytes zero initialized memory
 move     (pp)   : reference passing (pointer to pointer)
 destroy  (pp)   : reference passing (pointer to pointer)
└─────────────────────────────────────────────────────────────────────────────*/

static inline
pointer (void) (allocate) (size_t size)
{
    pointer (void) space = calloc (1, size);
    assert (space);
    return space;
}

static inline
pointer (void) (move) (pointer (void) p)
{
    pointer (pointer (void)) pp        = p;
    pointer (void)           ownership = NULL;
    if (pp)
        ownership = *pp, *pp = NULL;
    return ownership;
}

static inline
void (destroy) (pointer (void) p)
{
    pointer (pointer (void)) pp = p;
    if (pp)
        free (*pp), *pp = NULL;
}
