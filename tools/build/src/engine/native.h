/* Copyright 2003. David Abrahams
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#ifndef NATIVE_H_VP_2003_12_09
#define NATIVE_H_VP_2003_12_09

#include "config.h"
#include "function.h"
#include "frames.h"
#include "lists.h"
#include "object.h"

typedef struct native_rule_t
{
    OBJECT * name;
    FUNCTION * procedure;

    /* Version of the interface that the native rule provides. It is possible
     * that we want to change the set parameter for existing native rule. In
     * that case, version number should be incremented so B2 can check
     * for the version it relies on.
     *
     * Versions are numbered from 1.
    */
    int32_t version;
} native_rule_t;
/* MSVC debugger gets confused unless the native_rule_t typedef is provided. */

void declare_native_rule( char const * module, char const * rule,
    char const * * args, LIST * (*f)( FRAME *, int32_t ), int32_t version );

#endif
