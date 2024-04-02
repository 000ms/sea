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

#ifndef EXPERIMENTAL_LIST_H
#define EXPERIMENTAL_LIST_H

#include <experimental/memory.h>

typedef struct $Link
{
    struct $Link *forward;
    struct $Link *backward;
}
    $Link;

static inline //----------------------------------------------------------------
$Link * ($_link_forward)($Link *source)
{
    return source ? source->forward : NULL;
}

static inline //----------------------------------------------------------------
$Link * ($_link_backward)($Link *source)
{
    return source ? source->backward : NULL;
}

static inline //----------------------------------------------------------------
void ($_link_set_forward)($Link *source, $Link *destination)
{
    if (source)
        source->forward = destination;
}

static inline //----------------------------------------------------------------
void ($_link_set_backward)($Link *source, $Link *destination)
{
    if (source)
        source->backward = destination;
}

#define $_link_forward(_source_)                                                \
    ($_link_forward(($Link *)_source_))

#define $_link_backward(_source_)                                               \
    ($_link_backward(($Link *)_source_))

#define $_link_set_forward(_source_, _destination_)                             \
    ($_link_set_forward(($Link *)_source_, ($Link *)_destination_))

#define $_link_set_backward(_source_, _destination_)                            \
    ($_link_set_backward(($Link *)_source_, ($Link *)_destination_))

//▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓

#define $_list_new(_object_)                                                    \
({                                                                              \
    $ListOf(_object_) dummy;                                                    \
    $auto instance = $_memory_new(dummy);                                       \
    $_list_init(instance);                                                      \
    instance;                                                                   \
})

#define $ListOf(_object_)                                                       \
    typeof                                                                      \
    (                                                                           \
    struct $Head                                                                \
    {                                                                           \
        $Link super;                                                            \
        size_t length;                                                          \
                                                                                \
        struct $Node                                                            \
        {                                                                       \
            $Link super;                                                        \
            typeof(_object_) data;                                              \
        }                                                                       \
            *iterator;                                                          \
                                                                                \
        size_t         (*size)    (struct $Head *);                             \
        void           (*reset)   (struct $Head *);                             \
        struct $Node * (*forward) (struct $Head *);                             \
        struct $Node * (*backward)(struct $Head *);                             \
        struct $Node * (*append)  (struct $Head *);                             \
        struct $Node * (*prepend) (struct $Head *);                             \
        struct $Node * (*extract) (struct $Head *);                             \
    }                                                                           \
    )

#define $_list_init(_head_)                                                     \
({                                                                              \
    $auto dummy = _head_;                                                       \
                                                                                \
    typedef typeof(*dummy)           $Head;                                     \
    typedef typeof(*dummy->iterator) $Node;                                     \
                                                                                \
    instance->size = $_function(size_t, ($Head *head)                           \
    {                                                                           \
        assert(head);                                                           \
        return head->length;                                                    \
    });                                                                         \
                                                                                \
    instance->reset = $_function(void, ($Head *head)                            \
    {                                                                           \
        assert(head);                                                           \
        head->iterator = NULL;                                                  \
    });                                                                         \
                                                                                \
    instance->forward = $_function($Node *, ($Head *head)                       \
    {                                                                           \
        assert(head);                                                           \
        if (head->iterator)                                                     \
                head->iterator = ($Node *)$_link_forward(head->iterator);       \
        else    head->iterator = ($Node *)$_link_forward(head);                 \
        return  head->iterator;                                                 \
    });                                                                         \
                                                                                \
    instance->backward = $_function($Node *, ($Head *head)                      \
    {                                                                           \
        assert(head);                                                           \
        if (head->iterator)                                                     \
                head->iterator = ($Node *)$_link_backward(head->iterator);      \
        else    head->iterator = ($Node *)$_link_backward(head);                \
        return  head->iterator;                                                 \
    });                                                                         \
                                                                                \
    instance->append = $_function($Node *, ($Head *head)                        \
    {                                                                           \
        assert(head);                                                           \
        $auto node = $_memory_new(*head->iterator);                             \
        if (head->length) {                                                     \
            $_link_set_backward(                 node, $_link_backward(head));  \
            $_link_set_forward ($_link_backward(head), node                 ); }\
        else                                                                    \
            $_link_set_forward (                 head, node                 );  \
        $_link_set_backward(head, node);                                        \
        return (head->length ++, node);                                         \
    });                                                                         \
                                                                                \
    instance->prepend = $_function($Node *, ($Head *head)                       \
    {                                                                           \
        assert(head);                                                           \
        $auto node = $_memory_new(*head->iterator);                             \
        if (head->length) {                                                     \
            $_link_set_forward (               node , $_link_forward(head));    \
            $_link_set_backward($_link_forward(head), node                );   }\
        else                                                                    \
            $_link_set_backward(               head , node                );    \
        $_link_set_forward(head, node);                                         \
        return  (head->length ++, node);                                        \
    });                                                                         \
                                                                                \
    instance->extract = $_function($Node *, ($Head *head)                       \
    {                                                                           \
        assert(head);                                                           \
        $auto i = head->iterator;                                               \
        if (i)                                                                  \
        {                                                                       \
            if ($_link_backward(i))                                             \
                    $_link_set_forward($_link_backward(i), $_link_forward (i)); \
            else    $_link_set_forward(              head, $_link_forward (i)); \
            if ($_link_forward (i))                                             \
                    $_link_set_backward($_link_forward(i), $_link_backward(i)); \
            else    $_link_set_backward(             head, $_link_backward(i)); \
            if ($_link_backward(i))                                             \
                    head->iterator = ($Node *) $_link_backward(i);              \
            else    head->iterator = ($Node *) $_link_forward (i);              \
            $_link_set_backward(i, NULL);                                       \
            $_link_set_forward (i, NULL);                                       \
            head->length --;                                                    \
        }                                                                       \
        return i;                                                               \
    });                                                                         \
                                                                                \
    instance;                                                                   \
})

#define $list $auto $scoped($_list_free)

static inline //----------------------------------------------------------------
void ($_list_free)(void *reference)
{
    $Link **head = reference;
    $Link  *mark = NULL;
    $Link  *next = NULL;

    if (head)
        mark = *head, *head = NULL;

    while (mark)
        next = $_link_forward(mark), free(mark), mark = next;
}

#endif
