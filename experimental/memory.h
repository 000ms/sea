/*
░░ ISC License

░░ Copyright (c) 2024 Sugiton <https://sugiton.dev>

░░ Permission to use, copy, modify, and/or distribute this software for any
░░ purpose with or without fee is hereby granted, provided that the above
░░ copyright notice and this permission notice appear in all copies.

░░ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
░░ REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
░░ AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
░░ INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
░░ LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
░░ OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
░░ PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef EXPERIMENTAL_MEMORY_H
#define EXPERIMENTAL_MEMORY_H

#include <assert.h>
#include <iso646.h>
#include <stdlib.h>

#if not defined(__GNUC__)
#   include <experimental/ requires gnu c compiler (gcc, clang)>
#endif

#if defined(__clang__)
#   if __has_extension(blocks)
#       define $function(_prototype_)                                           \
            typeof(typeof(_prototype_) ^)

#       define $_function(_return_, _block_)                                    \
            (^ typeof(_return_) _block_)
#   else
#       include <experimental/ clang may require flag (-fblocks)>
#   endif
#else
#   define $function(_prototype_)                                               \
        typeof(typeof(_prototype_) *)

#   define $_function(_return_, _block_)                                        \
        ({typeof(_return_) _ _block_ _;})
#endif

#define $(_object_)                                                             \
    (_object_->data)

#define $_(_object_, _method_, ...)                                             \
    (_object_->_method_(_object_, ##__VA_ARGS__))

#define $auto                                                                   \
    __auto_type

#define $scoped(_callback_)                                                     \
    [[gnu::cleanup(_callback_)]]

#define $free                                                                   \
    $scoped($_memory_free) $auto

#define $_memory_new(_object_)                                                  \
    ((typeof(_object_) *)($_memory_new)(sizeof(_object_)))

#define $_memory_move(_pointer_)                                                \
    ((typeof(_pointer_))$_memory_move_raw(_pointer_))

#define $_memory_move_raw(_pointer_)                                            \
    (($_memory_move_raw)(&_pointer_))

#define $_memory_free(_pointer_)                                                \
    (($_memory_free)(&_pointer_))

#define $_array_last(_array_)                                                   \
    ($_array_size(_array_) - 1)

#define $_array_size(_array_)                                                   \
({                                                                              \
    static_assert                                                               \
        (not __builtin_types_compatible_p                                       \
                (typeof(_array_), typeof(&_array_[0]))                          \
        , "$_array_size requires an array type"                                 \
        );                                                                      \
    (sizeof(_array_) / sizeof(_array_[0]));                                     \
})

static inline //----------------------------------------------------------------
void * ($_memory_new)(size_t size)
{
    void *object = calloc(1, size);
    assert(object);
    return object;
}

static inline //----------------------------------------------------------------
void * ($_memory_move_raw)(void *reference)
{
    void **giver = reference;
    void  *taker = NULL;
    if (giver)
        taker = *giver, *giver = NULL;
    return taker;
}

static inline //----------------------------------------------------------------
void ($_memory_free)(void *reference)
{
    void **object = reference;
    if (object)
        free(*object), *object = NULL;
}

#endif
