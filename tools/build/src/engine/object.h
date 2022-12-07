/*
 * Copyright 2022 René Ferdinand Rivera Morell
 * Copyright 2011 Steven Watanabe
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * object.h - object manipulation routines
 */

#ifndef BOOST_JAM_OBJECT_H
#define BOOST_JAM_OBJECT_H

#include "config.h"
#include <string>
#include <cstring>

typedef struct _object OBJECT;

typedef OBJECT * object_ptr;

OBJECT * object_new( char const * const );
OBJECT * object_new_range( char const * const, int32_t size );
void object_done( void );

#if defined(NDEBUG) && !defined(BJAM_NO_MEM_CACHE)

struct hash_header
{
    unsigned int hash;
    struct hash_item * next;
};

#define object_str( obj ) ((char const *)(obj))
#define object_copy( obj ) (obj)
#define object_free( obj ) ((void)0)
#define object_equal( lhs, rhs ) ((lhs) == (rhs))
#define object_hash( obj ) (((struct hash_header *)((char *)(obj) - sizeof(struct hash_header)))->hash)

#else

char const * object_str  ( OBJECT * );
OBJECT *     object_copy ( OBJECT * );
void         object_free ( OBJECT * & );
int          object_equal( OBJECT *, OBJECT * );
unsigned int object_hash ( OBJECT * );

#endif

namespace b2 { namespace jam {

    struct object
    {
        inline object(const object &o)
            : obj(object_copy(o.obj)) {}

        inline explicit object(OBJECT *o)
            : obj(object_copy(o)) {}
        inline explicit object(const char * val)
            : obj(object_new(val)) {}
        inline explicit object(const std::string &val)
            : obj(object_new(val.c_str())) {}

        inline ~object() { if (obj) object_free(obj); }
        inline OBJECT * release() { OBJECT *r = obj; obj = nullptr; return r; }

        inline operator OBJECT*() const { return obj; }
        inline operator std::string() const { return object_str(obj); }

        inline bool operator==(OBJECT *o) const { return std::strcmp(object_str(obj), object_str(o)) == 0; }

        private:

        OBJECT * obj = nullptr;
    };

}}

#endif
