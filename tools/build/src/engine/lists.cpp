/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * lists.c - maintain lists of objects
 */

#include "jam.h"
#include "lists.h"
#include "mem.h"
#include "output.h"
#include "startup.h"

#include <assert.h>

static int32_t get_bucket( int32_t size )
{
    int32_t bucket = 0;
    while ( size > ( int32_t(1) << bucket ) ) ++bucket;
    return bucket;
}

static LIST * list_alloc( int32_t size )
{
    int32_t bucket = get_bucket( size );
    return b2::jam::ctor_ptr<LIST>( BJAM_CALLOC(
        1, sizeof( LIST ) + ( size_t( 1 ) << bucket ) * sizeof( OBJECT * ) ) );
}

static void list_dealloc( LIST * l )
{
    int32_t size = list_length( l );
    LIST * node = l;

    if ( size == 0 ) return;

    b2::jam::free_ptr( node );
}

/*
 * list_append() - append a list onto another one, returning total
 */

LIST * list_append( LIST * l, LIST * nl )
{
    if ( list_empty( l ) )
        return nl;
    if ( !list_empty( nl ) )
    {
        int32_t l_size = list_length( l );
        int32_t nl_size = list_length( nl );
        int32_t size = l_size + nl_size;
        int32_t bucket = get_bucket( size );

        /* Do we need to reallocate? */
        if ( l_size <= ( int32_t(1) << ( bucket - 1 ) ) )
        {
            LIST * result = list_alloc( size );
            memcpy( list_begin( result ), list_begin( l ), l_size * sizeof(
                OBJECT * ) );
            list_dealloc( l );
            l = result;
        }

        l->impl.size = size;
        memcpy( list_begin( l ) + l_size, list_begin( nl ), nl_size * sizeof(
            OBJECT * ) );
        list_dealloc( nl );
    }
    return l;
}

LISTITER list_begin( LIST * l )
{
    return l ? (LISTITER)( (char *)l + sizeof( LIST ) ) : 0;
}

LISTITER list_end( LIST * l )
{
    return l ? list_begin( l ) + l->impl.size : 0;
}

LIST * list_new( OBJECT * value )
{
    LIST * const head = list_alloc( 1 ) ;
    head->impl.size = 1;
    list_begin( head )[ 0 ] = value;
    return head;
}

/*
 * list_push_back() - tack a string onto the end of a list of strings
 */

LIST * list_push_back( LIST * head, OBJECT * value )
{
    int32_t size = list_length( head );

    if ( DEBUG_LISTS )
        out_printf( "list > %s <\n", object_str( value ) );

    /* If the size is a power of 2, reallocate. */
    if ( size == 0 )
    {
        head = list_alloc( 1 );
    }
    else if ( ( ( size - 1 ) & size ) == 0 )
    {
        LIST * l = list_alloc( size + 1 );
        memcpy( l, head, sizeof( LIST ) + size * sizeof( OBJECT * ) );
        list_dealloc( head );
        head = l;
    }

    list_begin( head )[ size ] = value;
    head->impl.size = size + 1;

    return head;
}


/*
 * list_copy() - copy a whole list of strings (nl) onto end of another (l).
 */

LIST * list_copy( LIST * l )
{
    int32_t size = list_length( l );
    int32_t i;
    LIST * result;

    if ( size == 0 ) return L0;

    result = list_alloc( size );
    result->impl.size = size;
    for ( i = 0; i < size; ++i )
        list_begin( result )[ i ] = object_copy( list_begin( l )[ i ] );
    return result;
}


LIST * list_copy_range( LIST * l, LISTITER first, LISTITER last )
{
    if ( first == last )
        return L0;
    else
    {
        int32_t size = int32_t( last - first );
        LIST * result = list_alloc( size );
        LISTITER dest = list_begin( result );
        result->impl.size = size;
        for ( ; first != last; ++first, ++dest )
            *dest = object_copy( *first );
        return result;
    }
}


/*
 * list_sublist() - copy a subset of a list of strings.
 */

LIST * list_sublist( LIST * l, int32_t start, int32_t count )
{
    int32_t end = start + count;
    int32_t size = list_length( l );
    if ( start >= size ) return L0;
    if ( end > size ) end = size;
    return list_copy_range( l, list_begin( l ) + start, list_begin( l ) + end );
}


static int32_t str_ptr_compare( void const * va, void const * vb )
{
    OBJECT * a = *( (OBJECT * *)va );
    OBJECT * b = *( (OBJECT * *)vb );
    return strcmp( object_str( a ), object_str( b ) );
}


LIST * list_sort( LIST * l )
{
    int32_t len;
    LIST * result;

    if ( !l )
        return L0;

    len = list_length( l );
    result = list_copy( l );

    qsort( list_begin( result ), len, sizeof( OBJECT * ), str_ptr_compare );

    return result;
}


/*
 * list_free() - free a list of strings
 */

void list_free( LIST * head )
{
    if ( !list_empty( head ) )
    {
        LISTITER iter = list_begin( head );
        LISTITER const end = list_end( head );
        for ( ; iter != end; iter = list_next( iter ) )
            object_free( list_item( iter ) );
        list_dealloc( head );
    }
}


/*
 * list_pop_front() - remove the front element from a list of strings
 */

LIST * list_pop_front( LIST * l )
{
    int32_t size = list_length( l );
    assert( size );
    --size;
    object_free( list_front( l ) );

    if ( size == 0 )
    {
        list_dealloc( l );
        return L0;
    }

    if ( ( ( size - 1 ) & size ) == 0 )
    {
        LIST * const nl = list_alloc( size );
        nl->impl.size = size;
        memcpy( list_begin( nl ), list_begin( l ) + 1, size * sizeof( OBJECT * )
            );
        list_dealloc( l );
        return nl;
    }

    l->impl.size = size;
    memmove( list_begin( l ), list_begin( l ) + 1, size * sizeof( OBJECT * ) );
    return l;
}

LIST * list_reverse( LIST * l )
{
    int32_t size = list_length( l );
    if ( size == 0 ) return L0;
    {
        LIST * const result = list_alloc( size );
        int32_t i;
        result->impl.size = size;
        for ( i = 0; i < size; ++i )
            list_begin( result )[ i ] = object_copy( list_begin( l )[ size - i -
                1 ] );
        return result;
    }
}

int32_t list_cmp( LIST * t, LIST * s )
{
    int32_t status = 0;
    LISTITER t_it = list_begin( t );
    LISTITER const t_end = list_end( t );
    LISTITER s_it = list_begin( s );
    LISTITER const s_end = list_end( s );

    while ( !status && ( t_it != t_end || s_it != s_end ) )
    {
        char const * st = t_it != t_end ? object_str( list_item( t_it ) ) : "";
        char const * ss = s_it != s_end ? object_str( list_item( s_it ) ) : "";

        status = strcmp( st, ss );

        t_it = t_it != t_end ? list_next( t_it ) : t_it;
        s_it = s_it != s_end ? list_next( s_it ) : s_it;
    }

    return status;
}

int32_t list_is_sublist( LIST * sub, LIST * l )
{
    LISTITER iter = list_begin( sub );
    LISTITER const end = list_end( sub );
    for ( ; iter != end; iter = list_next( iter ) )
        if ( !list_in( l, list_item( iter ) ) )
            return 0;
    return 1;
}

/*
 * list_print() - print a list of strings to stdout
 */

void list_print( LIST * l )
{
    LISTITER iter = list_begin( l ), end = list_end( l );
    if ( iter != end )
    {
        out_printf( "%s", object_str( list_item( iter ) ) );
        iter = list_next( iter );
        for ( ; iter != end; iter = list_next( iter ) )
            out_printf( " %s", object_str( list_item( iter ) ) );
    }
}


/*
 * list_length() - return the number of items in the list
 */

int32_t list_length( LIST * l )
{
    return l ? l->impl.size : 0;
}


int32_t list_in( LIST * l, OBJECT * value )
{
    LISTITER iter = list_begin( l );
    LISTITER end = list_end( l );
    for ( ; iter != end; iter = list_next( iter ) )
        if ( object_equal( list_item( iter ), value ) )
            return 1;
    return 0;
}


LIST * list_unique( LIST * sorted_list )
{
    LIST * result = L0;
    OBJECT * last_added = 0;

    LISTITER iter = list_begin( sorted_list ), end = list_end( sorted_list );
    for ( ; iter != end; iter = list_next( iter ) )
    {
        if ( !last_added || !object_equal( list_item( iter ), last_added ) )
        {
            result = list_push_back( result, object_copy( list_item( iter ) ) );
            last_added = list_item( iter );
        }
    }
    return result;
}

void list_done()
{
}


/*
 * lol_init() - initialize a LOL (list of lists).
 */

void lol_init( LOL * lol )
{
    lol->count = 0;
}


/*
 * lol_add() - append a LIST onto an LOL.
 */

void lol_add( LOL * lol, LIST * l )
{
    if ( lol->count < LOL_MAX )
    {
        lol->list[ lol->count++ ] = l;
        return;
    }

    err_printf( "lol_add failed due to reached limit of %d elements\n", LOL_MAX );
    b2::clean_exit( EXITBAD );
}


/*
 * lol_free() - free the LOL and its LISTs.
 */

void lol_free( LOL * lol )
{
    int32_t i;
    for ( i = 0; i < lol->count; ++i )
        list_free( lol->list[ i ] );
    lol->count = 0;
}


/*
 * lol_get() - return one of the LISTs in the LOL.
 */

LIST * lol_get( LOL * lol, int32_t i )
{
    return i < lol->count ? lol->list[ i ] : L0;
}


/*
 * lol_print() - debug print LISTS separated by ":".
 */

void lol_print( LOL * lol )
{
    int32_t i;
    for ( i = 0; i < lol->count; ++i )
    {
        if ( i )
            out_printf( " : " );
        list_print( lol->list[ i ] );
    }
}
