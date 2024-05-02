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

#include <sea/join.h>

/*──────────────────────────────────────────────────────────────────────────────┐
│                                 generic list                                  │
└──────────────────────────────────────────────────────────────────────────────*/

#define ListOf(_type_)                                                          \
    struct Head                                                                 \
    {                                                                           \
        Join   super;                                                           \
        size_t count;                                                           \
                                                                                \
        pointer                                                                 \
            (                                                                   \
            struct Node                                                         \
            {                                                                   \
                Join super;                                                     \
                typeof (_type_) data;                                           \
            }                                                                   \
            )   iterator;                                                       \
                                                                                \
        method (void                  (pointer (struct Head))) reset;           \
        method (pointer (struct Node) (pointer (struct Head))) forward;         \
        method (pointer (struct Node) (pointer (struct Head))) backward;        \
        method (pointer (struct Node) (pointer (struct Head))) append;          \
        method (pointer (struct Node) (pointer (struct Head))) prepend;         \
        method (pointer (struct Node) (pointer (struct Head))) extract;         \
    }

/*──────────────────────────────────────────────────────────────────────────────┐
│                                   allocator                                   │
└──────────────────────────────────────────────────────────────────────────────*/

#define allocatelistof(_type_)                                                  \
({                                                                              \
    ListOf (_type_) object;                                                     \
    inferred instance = allocate (object);                                      \
    initlist (instance);                                                        \
    instance;                                                                   \
})

/*──────────────────────────────────────────────────────────────────────────────┐
│                                  initializer                                  │
└──────────────────────────────────────────────────────────────────────────────*/

#define initlist(_head_)                                                        \
({                                                                              \
    inferred head = _head_;                                                     \
                                                                                \
    typedef typeof (*head)           Head;                                      \
    typedef typeof (*head->iterator) Node;                                      \
                                                                                \
    instance->reset = routine (void, (pointer (Head) head)                      \
    {                                                                           \
        assert (head);                                                          \
        head->iterator = NULL;                                                  \
    });                                                                         \
                                                                                \
    instance->forward = routine (pointer (Node), (pointer (Head) head)          \
    {                                                                           \
        assert (head);                                                          \
        if (head->iterator)                                                     \
                head->iterator = (pointer (Node))to (head->iterator);           \
        else    head->iterator = (pointer (Node))to (head);                     \
        return  head->iterator;                                                 \
    });                                                                         \
                                                                                \
    instance->backward = routine (pointer (Node), (pointer (Head) head)         \
    {                                                                           \
        assert (head);                                                          \
        if (head->iterator)                                                     \
                head->iterator = (pointer (Node))from (head->iterator);         \
        else    head->iterator = (pointer (Node))from (head);                   \
        return  head->iterator;                                                 \
    });                                                                         \
                                                                                \
    instance->append = routine (pointer (Node), (pointer (Head) head)           \
    {                                                                           \
        assert (head);                                                          \
        inferred node = allocate (Node);                                        \
        if (head->count)                        {                               \
            setfrom (      node , from (head));                                 \
            setto   (from (head), node       ); }                               \
        else                                    {                               \
            setto   (      head , node       ); }                               \
        setfrom (head, node);                                                   \
        head->count++;                                                          \
        return node;                                                            \
    });                                                                         \
                                                                                \
    instance->prepend = routine (pointer (Node), (pointer (Head) head)          \
    {                                                                           \
        assert (head);                                                          \
        inferred node = allocate (Node);                                        \
        if (head->count)                    {                                   \
            setto   (    node , to (head));                                     \
            setfrom (to (head), node     ); }                                   \
        else                                {                                   \
            setfrom (    head , node     ); }                                   \
        setto (head, node);                                                     \
        head->count++;                                                          \
        return node;                                                            \
    });                                                                         \
                                                                                \
    instance->extract = routine (pointer (Node), (pointer (Head) head)          \
    {                                                                           \
        assert (head);                                                          \
        inferred it = head->iterator;                                           \
        if (it)                                                                 \
        {                                                                       \
            if (from (it))                                                      \
                    setto (from (it), to (it));                                 \
            else    setto (     head, to (it));                                 \
            if (to (it))                                                        \
                    setfrom (to (it), from (it));                               \
            else    setfrom (   head, from (it));                               \
            if (from (it))                                                      \
                    head->iterator = (pointer (Node))from (it);                 \
            else    head->iterator = (pointer (Node))to   (it);                 \
            setfrom (it, NULL);                                                 \
            setto   (it, NULL);                                                 \
            head->count--;                                                      \
        }                                                                       \
        return it;                                                              \
    });                                                                         \
                                                                                \
    instance;                                                                   \
})

/*──────────────────────────────────────────────────────────────────────────────┐
│                           scoped handler of a list                            │
└──────────────────────────────────────────────────────────────────────────────*/

#define list scoped (destroylist) inferred

static inline
void (destroylist) (pointer (void) p)
{
    pointer (pointer (Join)) head = p;
    pointer (Join)           mark = NULL;
    pointer (Join)           next = NULL;

    if (head)
        mark = *head, *head = NULL;

    while (mark)
        next = to (mark), free (mark), mark = next;
}
