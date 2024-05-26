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

#include <assert.h>
#include <iso646.h>
#include <stdbool.h>

/*─────────────────────────────────────────────────────────────────────────────┐
 method (prototype)        : [gcc] function pointer [clang] block type
 lambda (return, argsbody) : [gcc] nested function  [clang] block literal
└─────────────────────────────────────────────────────────────────────────────*/

#if not defined __GNUC__
    static_assert (false, "FIX : use gnu c compiler");
#endif

#if defined __clang__ and not __has_extension(blocks)
    static_assert (false, "FIX : use clang -fblocks extension");
#endif

#if defined __clang__
#   define method(_prototype_)                                                 \
        typeof (typeof (_prototype_) ^)

#   define lambda(_return_, _body_)                                            \
        (^ typeof (_return_) _body_)
#else
#   define method(_prototype_)                                                 \
        typeof (typeof (_prototype_) *)

#   define lambda(_return_, _body_)                                            \
        ({typeof (_return_) _ _body_ _;})
#endif

/*─────────────────────────────────────────────────────────────────────────────┐
 scoped (callback) : variable attribute invokes callback at scope exit
└─────────────────────────────────────────────────────────────────────────────*/

#define scoped(_callback_)                                                     \
    __attribute__ ((cleanup (_callback_)))

/*─────────────────────────────────────────────────────────────────────────────┐
 inferred : type inference requires initializer
└─────────────────────────────────────────────────────────────────────────────*/

#define inferred                                                               \
    __auto_type

/*─────────────────────────────────────────────────────────────────────────────┐
 pointer (type)       : verbose pointer
 array   (type, size) : verbose array
└─────────────────────────────────────────────────────────────────────────────*/

#define pointer(_type_)                                                        \
    typeof (typeof (_type_) *)

#define array(_type_, _size_)                                                  \
    typeof (typeof (_type_) [_size_])

/*─────────────────────────────────────────────────────────────────────────────┐
 arraylast (array) : array's last index
 arraysize (array) : array's length
└─────────────────────────────────────────────────────────────────────────────*/

#define arraylast(_array_)                                                     \
    (arraysize (_array_) - 1)

#define arraysize(_array_)                                                     \
({                                                                             \
    static_assert                                                              \
        (not __builtin_types_compatible_p                                      \
                (typeof (_array_), typeof (&0 [_array_]))                      \
        , "TIP : use pointer to array to prevent array decay"                  \
        );                                                                     \
    (sizeof (_array_) / sizeof (0 [_array_]));                                 \
})

/*─────────────────────────────────────────────────────────────────────────────┐
 self (object, method (, args)) : self aware object's method call
└─────────────────────────────────────────────────────────────────────────────*/

#define self(_object_, _method_, ...)                                          \
    ((*_object_)._method_ (_object_, ##__VA_ARGS__))
