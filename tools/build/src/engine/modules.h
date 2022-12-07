/*
 *  Copyright 2022 René Ferdinand Rivera Morell
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */
#ifndef MODULES_DWA10182001_H
#define MODULES_DWA10182001_H

#include "config.h"
#include "lists.h"
#include "object.h"

typedef struct module_t module_t ;

typedef module_t * module_ptr;

struct module_t
{
    OBJECT * name;
    struct hash * rules;
    struct hash * variables;
    struct hash * variable_indices;
    int num_fixed_variables;
    LIST * * fixed_variables;
    struct hash * imported_modules;
    module_t * class_module;
    struct hash * native_rules;
    int user_module;
};

module_t * bindmodule( OBJECT * name );
module_t * root_module();
void delete_module( module_t * );

void import_module( LIST * module_names, module_t * target_module );
LIST * imported_modules( module_t * );

struct hash * demand_rules( module_t * );

void module_bind_variables( module_t * );

/*
 * After calling module_add_fixed_var, module_set_fixed_variables must be called
 * before accessing any variables in the module.
 */
int module_add_fixed_var( module_t *, OBJECT * name, int * n );
void module_set_fixed_variables( module_t *, int n );

/*
 * Returns the index of the variable or -1 if none exists.
 */
int module_get_fixed_var( module_t *, OBJECT * name );

void modules_done();

#endif
