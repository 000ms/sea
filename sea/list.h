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

#include <sea/join.h>

/*─────────────────────────────────────────────────────────────────────────────┐
 ListOf (type) : defines Head < Join and Node < Join with typed data field
└─────────────────────────────────────────────────────────────────────────────*/

#define ListOf(_type_)                                                         \
    struct Head                                                                \
    {                                                                          \
        Join   super;                                                          \
        size_t count;                                                          \
                                                                               \
        pointer                                                                \
            (                                                                  \
            struct Node                                                        \
            {                                                                  \
                Join super;                                                    \
                typeof (_type_) data;                                          \
            }                                                                  \
            )   iterator;                                                      \
                                                                               \
        method (void                  (pointer (struct Head))) reset;          \
        method (pointer (struct Node) (pointer (struct Head))) forward;        \
        method (pointer (struct Node) (pointer (struct Head))) backward;       \
        method (pointer (struct Node) (pointer (struct Head))) append;         \
        method (pointer (struct Node) (pointer (struct Head))) prepend;        \
        method (pointer (struct Node) (pointer (struct Head))) extract;        \
    }

/*─────────────────────────────────────────────────────────────────────────────┐
 allocatelistof (type) : allocates and initializes a list of (type)
└─────────────────────────────────────────────────────────────────────────────*/

#define allocatelistof(_type_)                                                 \
({                                                                             \
    ListOf (_type_) object;                                                    \
    inferred instance = allocate (object);                                     \
    initlist (instance);                                                       \
    instance;                                                                  \
})

/*─────────────────────────────────────────────────────────────────────────────┐
 initlist (list) : instantiates the list's methods
    the list contains a current node pointer also iterator on which
    reset, forward, backward and extract acts.
    - reset    (list) : sets iterator to initial position
    - forward  (list) : sets iterator to the next link
    - backward (list) : sets iterator to the previous link
    - append   (list) : allocates a new node at the end
    - prepend  (list) : allocates a new node at the beginning
    - extract  (list) : returns the current node
                      , iterator is set either to previous or next in order
└─────────────────────────────────────────────────────────────────────────────*/

#define initlist(_head_)                                                       \
({                                                                             \
    inferred head = _head_;                                                    \
                                                                               \
    typedef typeof (*head)           Head;                                     \
    typedef typeof (*head->iterator) Node;                                     \
                                                                               \
    head->reset = lambda (void, (pointer (Head) head)                          \
    {                                                                          \
        assert (head);                                                         \
        head->iterator = NULL;                                                 \
    });                                                                        \
                                                                               \
    head->forward = lambda (pointer (Node), (pointer (Head) head)              \
    {                                                                          \
        assert (head);                                                         \
        if (head->iterator)                                                    \
                head->iterator = (pointer (Node))to (head->iterator);          \
        else    head->iterator = (pointer (Node))to (head);                    \
        return  head->iterator;                                                \
    });                                                                        \
                                                                               \
    head->backward = lambda (pointer (Node), (pointer (Head) head)             \
    {                                                                          \
        assert (head);                                                         \
        if (head->iterator)                                                    \
                head->iterator = (pointer (Node))from (head->iterator);        \
        else    head->iterator = (pointer (Node))from (head);                  \
        return  head->iterator;                                                \
    });                                                                        \
                                                                               \
    head->append = lambda (pointer (Node), (pointer (Head) head)               \
    {                                                                          \
        assert (head);                                                         \
        inferred node = allocate (Node);                                       \
        if (head->count)                        {                              \
            setfrom (      node , from (head));                                \
            setto   (from (head), node       ); }                              \
        else                                    {                              \
            setto   (      head , node       ); }                              \
        setfrom (head, node);                                                  \
        head->count++;                                                         \
        return node;                                                           \
    });                                                                        \
                                                                               \
    head->prepend = lambda (pointer (Node), (pointer (Head) head)              \
    {                                                                          \
        assert (head);                                                         \
        inferred node = allocate (Node);                                       \
        if (head->count)                    {                                  \
            setto   (    node , to (head));                                    \
            setfrom (to (head), node     ); }                                  \
        else                                {                                  \
            setfrom (    head , node     ); }                                  \
        setto (head, node);                                                    \
        head->count++;                                                         \
        return node;                                                           \
    });                                                                        \
                                                                               \
    head->extract = lambda (pointer (Node), (pointer (Head) head)              \
    {                                                                          \
        assert (head);                                                         \
        inferred it = head->iterator;                                          \
        if (it)                                                                \
        {                                                                      \
            if (from (it))                                                     \
                    setto (from (it), to (it));                                \
            else    setto (     head, to (it));                                \
            if (to (it))                                                       \
                    setfrom (to (it), from (it));                              \
            else    setfrom (   head, from (it));                              \
            if (from (it))                                                     \
                    head->iterator = (pointer (Node))from (it);                \
            else    head->iterator = (pointer (Node))to   (it);                \
            setfrom (it, NULL);                                                \
            setto   (it, NULL);                                                \
            head->count--;                                                     \
        }                                                                      \
        return it;                                                             \
    });                                                                        \
                                                                               \
    head;                                                                      \
})

/*─────────────────────────────────────────────────────────────────────────────┐
 list : inferred scoped-bound list handler
└─────────────────────────────────────────────────────────────────────────────*/

#define list scoped (destroylist) inferred

/*─────────────────────────────────────────────────────────────────────────────┐
 destroylist (pp) : erases dangling list's address and frees each link
└─────────────────────────────────────────────────────────────────────────────*/

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
