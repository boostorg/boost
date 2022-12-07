/*
 * Copyright 2004. David Abrahams
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#ifndef JAM_STRINGS_DWA20011024_H
#define JAM_STRINGS_DWA20011024_H

#include "config.h"
#include <stddef.h>

typedef struct string
{
    char * value;
    int32_t size;
    int32_t capacity;
    char opt[ 32 ];
#ifndef NDEBUG
    char magic[ 4 ];
#endif
} string;

void string_new( string * );
void string_copy( string *, char const * );
void string_free( string * );
void string_append( string *, char const * );
void string_append_range( string *, char const *, char const * );
void string_push_back( string * s, char x );
void string_reserve( string *, int32_t );
void string_truncate( string *, int32_t );
void string_pop_back( string * );
char string_back( string * );
void string_rtrim( string * );
void string_unit_test();

#endif
