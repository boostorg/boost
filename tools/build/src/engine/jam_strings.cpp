/* Copyright David Abrahams 2004. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE.txt or copy at https://www.bfgroup.xyz/b2/LICENSE.txt) */

#include "jam.h"
#include "jam_strings.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


#ifndef NDEBUG
# define JAM_STRING_MAGIC ((char)0xcf)
# define JAM_STRING_MAGIC_SIZE 4
static void assert_invariants( string * self )
{
    int i;

    if ( self->value == 0 )
    {
        assert( self->size == 0 );
        assert( self->capacity == 0 );
        assert( self->opt[ 0 ] == 0 );
        return;
    }

    assert( self->size < self->capacity );
    assert( ( self->capacity <= int32_t(sizeof( self->opt )) ) == ( self->value == self->opt ) );
    assert( self->value[ self->size ] == 0 );
    /* String objects modified manually after construction to contain embedded
     * '\0' characters are considered structurally valid.
     */
    assert( strlen( self->value ) <= size_t(self->size) );

    for ( i = 0; i < 4; ++i )
    {
        assert( self->magic[ i ] == JAM_STRING_MAGIC );
        assert( self->value[ self->capacity + i ] == JAM_STRING_MAGIC );
    }
}
#else
# define JAM_STRING_MAGIC_SIZE 0
# define assert_invariants(x) do {} while (0)
#endif


void string_new( string * s )
{
    s->value = s->opt;
    s->size = 0;
    s->capacity = sizeof( s->opt );
    s->opt[ 0 ] = 0;
#ifndef NDEBUG
    memset( s->magic, JAM_STRING_MAGIC, sizeof( s->magic ) );
#endif
    assert_invariants( s );
}


void string_free( string * s )
{
    assert_invariants( s );
    if ( s->value != s->opt )
        BJAM_FREE( s->value );
    string_new( s );
}


static void string_reserve_internal( string * self, int32_t capacity )
{
    if ( self->value == self->opt )
    {
        self->value = (char *)BJAM_MALLOC_ATOMIC( size_t(capacity) +
            JAM_STRING_MAGIC_SIZE );
        self->value[ 0 ] = 0;
        size_t opt_size = sizeof(self->opt); // Workaround sizeof in strncat warning.
        strncat( self->value, self->opt, opt_size );
        assert( strlen( self->value ) <= size_t(self->capacity) && "Regression test" );
    }
    else
    {
        self->value = (char *)BJAM_REALLOC( self->value, size_t(capacity) +
            JAM_STRING_MAGIC_SIZE );
    }
#ifndef NDEBUG
    memcpy( self->value + capacity, self->magic, JAM_STRING_MAGIC_SIZE );
#endif
    self->capacity = capacity;
}


void string_reserve( string * self, int32_t capacity )
{
    assert_invariants( self );
    if ( capacity <= self->capacity )
        return;
    string_reserve_internal( self, capacity );
    assert_invariants( self );
}


static void maybe_reserve( string * self, int32_t new_size )
{
    int32_t capacity = self->capacity;
    if ( capacity <= new_size )
    {
        int32_t new_capacity = capacity;
        while ( new_capacity <= new_size )
            new_capacity <<= 1;
        string_reserve_internal( self, new_capacity );
    }
}


void string_append( string * self, char const * rhs )
{
    int32_t rhs_size = int32_t(strlen( rhs ));
    int32_t new_size = self->size + rhs_size;
    assert_invariants( self );

    maybe_reserve( self, new_size );

    memcpy( self->value + self->size, rhs, size_t(rhs_size) + 1 );
    self->size = new_size;

    assert_invariants( self );
}


void string_append_range( string * self, char const * start, char const * finish )
{
    int32_t rhs_size = int32_t(finish - start);
    int32_t new_size = self->size + rhs_size;
    assert_invariants( self );

    maybe_reserve( self, new_size );

    if ( start != finish )
        memcpy( self->value + self->size, start, size_t(rhs_size) );
    self->size = new_size;
    self->value[ new_size ] = 0;

    assert_invariants( self );
}


void string_copy( string * s, char const * rhs )
{
    string_new( s );
    string_append( s, rhs );
}

void string_truncate( string * self, int32_t n )
{
    assert_invariants( self );
    assert( n <= self->capacity );
    self->value[ self->size = n ] = 0;
    assert_invariants( self );
}


void string_pop_back( string * self )
{
    string_truncate( self, self->size - 1 );
}


void string_push_back( string * self, char x )
{
    string_append_range( self, &x, &x + 1 );
}


char string_back( string * self )
{
    assert_invariants( self );
    return self->value[ self->size - 1 ];
}

void string_rtrim( string * self )
{
    char *p;
    assert_invariants( self );
    p = self->value + self->size - 1;
    for ( ; p >= self->value && ( *p == '\0' || isspace( *p ) ); *p-- = 0 );
}

#ifndef NDEBUG
void string_unit_test()
{
    {
        string s[ 1 ];
        unsigned long i;
        unsigned long const limit = sizeof( s->opt ) * 2 + 2;
        string_new( s );
        assert( s->value == s->opt );
        for ( i = 0; i < limit; ++i )
        {
            string_push_back( s, (char)( i + 1 ) );
            assert( s->size == int32_t(i + 1) );
        }
        assert( s->size == int32_t(limit) );
        assert( s->value != s->opt );
        for ( i = 0; i < limit; ++i )
            assert( s->value[ i ] == (char)( i + 1 ) );
        string_free( s );
    }

    {
        const char * const original = "  \n\t\v  Foo \r\n\v \tBar\n\n\r\r\t\n\v\t \t";
        string copy[ 1 ];
        string_copy( copy, original );
        assert( !strcmp( copy->value, original ) );
        assert( copy->size == int32_t(strlen( original )) );
        string_free( copy );
    }

    {
        const char * const foo = "Foo    ";
        string foo_copy[ 1 ];
        string_copy( foo_copy, foo );
        string_rtrim( foo_copy );
        assert( !strcmp( foo_copy->value, "Foo" ) );

        string_rtrim( foo_copy );
        assert( !strcmp( foo_copy->value, "Foo" ) );
    }
    {
        const char * const bar = "Bar\0\0\0";
        string bar_copy[ 1 ];
        string_copy( bar_copy, bar );
        string_rtrim( bar_copy );
        assert( !strcmp( bar_copy->value, "Bar" ) );

        string_rtrim( bar_copy );
        assert( !strcmp( bar_copy->value, "Bar" ) );
    }
}
#endif
