/*──────────────────────────────────────────────────────────────────────────────┐
│ ISC License                                                                   │
│                                                                               │
│ Copyright (c) 2024 Sugiton <https://sugiton.dev>                              │
│                                                                               │
│ Permission to use, copy, modify, and/or distribute this software for any      │
│ purpose with or without fee is hereby granted, provided that the above        │
│ copyright notice and this permission notice appear in all copies.             │
│                                                                               │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH │
│ REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY   │
│ AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,  │
│ INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM   │
│ LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR │
│ OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR        │
│ PERFORMANCE OF THIS SOFTWARE.                                                 │
└──────────────────────────────────────────────────────────────────────────────*/

#pragma once

#include <see/memory.h>

/*──────────────────────────────────────────────────────────────────────────────┐
│                    a join from something to something else                    │
└──────────────────────────────────────────────────────────────────────────────*/

typedef struct Join
{
    pointer (struct Join) to;
    pointer (struct Join) from;
}
    Join;

/*──────────────────────────────────────────────────────────────────────────────┐
│                                upcasting masks                                │
└──────────────────────────────────────────────────────────────────────────────*/

#define to(_origin_)                                                            \
    (to ((pointer (Join))_origin_))

#define from(_origin_)                                                          \
    (from ((pointer (Join))_origin_))

#define setto(_origin_, _target_)                                               \
    (setto ((pointer (Join))_origin_, (pointer (Join))_target_))

#define setfrom(_origin_, _target_)                                             \
    (setfrom ((pointer (Join))_origin_, (pointer (Join))_target_))

/*──────────────────────────────────────────────────────────────────────────────┐
│                                   functions                                   │
└──────────────────────────────────────────────────────────────────────────────*/

static inline
pointer (Join) (to) (pointer (Join) origin)
{
    return origin ? origin->to : NULL;
}

static inline
pointer (Join) (from) (pointer (Join) origin)
{
    return origin ? origin->from : NULL;
}

static inline
void (setto) (pointer (Join) origin, pointer (Join) target)
{
    if (origin)
        origin->to = target;
}

static inline
void (setfrom) (pointer (Join) origin, pointer (Join) target)
{
    if (origin)
        origin->from = target;
}
