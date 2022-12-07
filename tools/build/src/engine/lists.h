/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2022 René Ferdinand Rivera Morell
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

/*
 * lists.h - the LIST structure and routines to manipulate them
 *
 * The whole of jam relies on lists of objects as a datatype. This module, in
 * conjunction with object.c, handles these relatively efficiently.
 *
 * Structures defined:
 *
 *  LIST - list of OBJECTs
 *  LOL - list of LISTs
 *
 * External routines:
 *
 *  list_append() - append a list onto another one, returning total
 *  list_new() - tack an object onto the end of a list of objects
 *  list_copy() - copy a whole list of objects
 *  list_sublist() - copy a subset of a list of objects
 *  list_free() - free a list of objects
 *  list_print() - print a list of objects to stdout
 *  list_length() - return the number of items in the list
 *
 *  lol_init() - initialize a LOL (list of lists)
 *  lol_add() - append a LIST onto an LOL
 *  lol_free() - free the LOL and its LISTs
 *  lol_get() - return one of the LISTs in the LOL
 *  lol_print() - debug print LISTS separated by ":"
 */

#ifndef LISTS_DWA20011022_H
#define LISTS_DWA20011022_H

#include "config.h"
#include "object.h"

/*
 * LIST - list of strings
 */

struct LIST {
    union {
        int32_t size;
        struct LIST * next;
        OBJECT * align;
    } impl;

    LIST()
    {
        this->impl.next = nullptr;
    }
};

typedef LIST * list_ptr;
typedef OBJECT * * LISTITER;

/*
 * LOL - list of LISTs
 */

#define LOL_MAX 19
typedef struct _lol {
    int32_t count;
    LIST * list[ LOL_MAX ];
} LOL;

LIST * list_new( OBJECT * value );
LIST * list_append( LIST * destination, LIST * source );
LIST * list_copy( LIST * );
LIST * list_copy_range( LIST * destination, LISTITER first, LISTITER last );
void   list_free( LIST * head );
LIST * list_push_back( LIST * head, OBJECT * value );
void   list_print( LIST * );
int32_t list_length( LIST * );
LIST * list_sublist( LIST *, int32_t start, int32_t count );
LIST * list_pop_front( LIST * );
LIST * list_sort( LIST * );
LIST * list_unique( LIST * sorted_list );
int32_t list_in( LIST *, OBJECT * value );
LIST * list_reverse( LIST * );
int32_t list_cmp( LIST * lhs, LIST * rhs );
int32_t list_is_sublist( LIST * sub, LIST * l );
void   list_done();

LISTITER list_begin( LIST * );
LISTITER list_end( LIST * );
#define list_next( it ) ((it) + 1)
#define list_item( it ) (*(it))
#define list_empty( l ) ((l) == L0)
#define list_front( l ) list_item( list_begin( l ) )

#define L0 ((LIST *)0)

void   lol_add( LOL *, LIST * );
void   lol_init( LOL * );
void   lol_free( LOL * );
LIST * lol_get( LOL *, int i );
void   lol_print( LOL * );
void   lol_build( LOL *, char const * * elements );

namespace b2 { namespace jam {
    struct list
    {
        struct iterator
        {
            inline explicit iterator(LISTITER i) : list_i(i) {}

            inline iterator operator++()
            {
                list_i = list_next(list_i);
                return *this;
            }
            inline iterator operator++(int)
            {
                iterator result{*this};
                list_i = list_next(list_i);
                return result;
            }
            inline bool operator==(iterator other) const { return list_i == other.list_i; }
            inline bool operator!=(iterator other) const { return list_i != other.list_i; }
            inline OBJECT *& operator*() const { return list_item(list_i); }
            inline OBJECT ** operator->() const { return &list_item(list_i); }

            private:

            LISTITER list_i;
        };

        friend struct iterator;

        inline list() = default;
        inline list(const list &other)
            : list_obj(list_copy(other.list_obj)) {}
        inline explicit list(const object &o)
            : list_obj(list_new(object_copy(o))) {}
        inline explicit list(LIST *l, bool own = false)
            : list_obj(own ? l : list_copy(l)) {}

        inline ~list() { reset(); }
        inline LIST* release()
        {
            LIST* r = list_obj;
            list_obj = nullptr;
            return r;
        }

        inline iterator begin() { return iterator(list_begin(list_obj)); }
        inline iterator end() { return iterator(list_end(list_obj)); }
        inline bool empty() const { return list_empty(list_obj) || length() == 0; }
        inline int32_t length() const { return list_length(list_obj); }
        inline list &append(const list &other)
        {
            list_obj = list_append(list_obj, list_copy(other.list_obj));
            return *this;
        }
        inline LIST* operator*() { return list_obj; }
        inline void reset(LIST * new_list = nullptr)
        {
            std::swap( list_obj, new_list );
            if (new_list) list_free(new_list);
        }
        inline list& pop_front()
        {
            list_obj = list_pop_front(list_obj);
            return *this;
        }

        private:

        LIST *list_obj = nullptr;
    };
}}

#endif
