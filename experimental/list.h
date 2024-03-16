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

#ifndef EXPERIMENTAL_LIST_H
#define EXPERIMENTAL_LIST_H

#include <experimental/memory.h>

//──────────────────────────────────────────────────────────────────────────────

typedef struct link
    {
    struct link *forward;
    struct link *backward;
    }
    link;

static inline link *(linkforward)(source)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    link *source;
    {
    return source ? source->forward : NULL;
    }

static inline link *(linkbackward)(source)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    link *source;
    {
    return source ? source->backward : NULL;
    }

static inline void (linksetforward)(source, destination)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    link *source;
    link *destination;
    {
    if (source)
        source->forward = destination;
    }

static inline void (linksetbackward)(source, destination)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    link *source;
    link *destination;
    {
    if (source)
        source->backward = destination;
    }

#define linkforward(source)                                                     \
    (linkforward((link*) source))

#define linkbackward(source)                                                    \
    (linkbackward((link*) source))

#define linksetforward(source, destination)                                     \
    (linksetforward( (link*) source                                             \
                   , (link*) destination ))

#define linksetbackward(source, destination)                                    \
    (linksetbackward( (link*) source                                            \
                    , (link*) destination ))

//──────────────────────────────────────────────────────────────────────────────

#define allocatelistof(type)                                                    \
    ({                                                                          \
    listof(type) listoftype;                                                    \
    autotype instance = allocate(listoftype);                                   \
    listinit(instance);                                                         \
    instance;                                                                   \
    })

#define listof(type)                                                            \
    typeof                                                                      \
    (                                                                           \
    struct head                                                                 \
        {                                                                       \
        struct link super;                                                      \
        size_t length;                                                          \
        struct node                                                             \
            {                                                                   \
            struct link super;                                                  \
            typeof(type) data;                                                  \
            }                                                                   \
            *iterator;                                                          \
                                                                                \
        size_t       (*size)    (struct head *);                                \
        void         (*reset)   (struct head *);                                \
        struct node *(*forward) (struct head *);                                \
        struct node *(*backward)(struct head *);                                \
        struct node *(*append)  (struct head *);                                \
        struct node *(*prepend) (struct head *);                                \
        struct node *(*extract) (struct head *);                                \
        }                                                                       \
    )

#define listinit(handler)                                                       \
    ({                                                                          \
    autotype instance = handler;                                                \
                                                                                \
    typedef typeof(*instance)           list;                                   \
    typedef typeof(*instance->iterator) node;                                   \
                                                                                \
    instance->size = function(size_t, (head)                                    \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        return head->length;                                                    \
        });                                                                     \
                                                                                \
    instance->reset = function(void, (head)                                     \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        head->iterator = NULL;                                                  \
        });                                                                     \
                                                                                \
    instance->forward = function(node *, (head)                                 \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        if (head->iterator)                                                     \
                head->iterator = (node *) linkforward(head->iterator);          \
        else    head->iterator = (node *) linkforward(head);                    \
        return  head->iterator;                                                 \
        });                                                                     \
                                                                                \
    instance->backward = function(node *, (head)                                \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        if (head->iterator)                                                     \
                head->iterator = (node *) linkbackward(head->iterator);         \
        else    head->iterator = (node *) linkbackward(head);                   \
        return  head->iterator;                                                 \
        });                                                                     \
                                                                                \
    instance->append = function(node *, (head)                                  \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        autotype novel = allocate(*head->iterator);                             \
        if (head->length)                                                       \
                {                                                               \
                linksetbackward(             novel, linkbackward(head));        \
                linksetforward (linkbackward(head), novel             );        \
                }                                                               \
        else    linksetforward (head , novel);                                  \
                linksetbackward(head , novel);                                  \
        return  (head->length ++, novel);                                       \
        });                                                                     \
                                                                                \
    instance->prepend = function(node *, (head)                                 \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        autotype novel = allocate(*head->iterator);                             \
        if (head->length)                                                       \
                {                                                               \
                linksetforward (            novel, linkforward(head));          \
                linksetbackward(linkforward(head), novel            );          \
                }                                                               \
        else    linksetbackward(head , novel);                                  \
                linksetforward (head , novel);                                  \
        return  (head->length ++, novel);                                       \
        });                                                                     \
                                                                                \
    instance->extract = function(node *, (head)                                 \
        list *head;                                                             \
        {                                                                       \
        assert(head);                                                           \
        autotype i = head->iterator;                                            \
        if (i)                                                                  \
            {                                                                   \
            if (linkbackward(i))                                                \
                    linksetforward(linkbackward(i), linkforward (i));           \
            else    linksetforward(           head, linkforward (i));           \
            if (linkforward (i))                                                \
                    linksetbackward(linkforward(i), linkbackward(i));           \
            else    linksetbackward(          head, linkbackward(i));           \
            if (linkbackward(i))                                                \
                    head->iterator = (node *) linkbackward(i);                  \
            else    head->iterator = (node *) linkforward (i);                  \
            linksetbackward(i, NULL);                                           \
            linksetforward (i, NULL);                                           \
            head->length --;                                                    \
            }                                                                   \
        return i;                                                               \
        });                                                                     \
                                                                                \
    instance;                                                                   \
    })

#define listsize(    list) (list->size    (list))
#define listreset(   list) (list->reset   (list))
#define listappend(  list) (list->append  (list))
#define listprepend( list) (list->prepend (list))
#define listextract( list) (list->extract (list))
#define listforward( list) (list->forward (list))
#define listbackward(list) (list->backward(list))

#define autolist autotype scoped(listdestroy)

static inline void (listdestroy)(pointer)
//▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
    void *pointer;
    {
    void **list = pointer;
    link  *mark = NULL;
    link  *next = NULL;

    if (list)
        mark = *list, *list = NULL;

    while (mark)
        next = linkforward(mark), free(mark), mark = next;
    }

#endif
