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
 * rules.c - access to RULEs, TARGETs, and ACTIONs
 *
 * External routines:
 *    bindrule()     - return pointer to RULE, creating it if necessary.
 *    bindtarget()   - return pointer to TARGET, creating it if necessary.
 *    touch_target() - mark a target to simulate being new.
 *    targetlist()   - turn list of target names into a TARGET chain.
 *    targetentry()  - add a TARGET to a chain of TARGETS.
 *    actionlist()   - append to an ACTION chain.
 *    addsettings()  - add a deferred "set" command to a target.
 *    pushsettings() - set all target specific variables.
 *    popsettings()  - reset target specific variables to their pre-push values.
 *    freesettings() - delete a settings list.
 *    rules_done()   - free RULE and TARGET tables.
 */

#include "jam.h"
#include "rules.h"

#include "hash.h"
#include "lists.h"
#include "object.h"
#include "output.h"
#include "parse.h"
#include "pathsys.h"
#include "search.h"
#include "variable.h"


static void set_rule_actions( rule_ptr, rule_actions_ptr );
static void set_rule_body   ( rule_ptr, function_ptr );

static struct hash * targethash = 0;


/*
 * get_target_includes() - lazy creates a target's internal includes node
 *
 * The newly created node is not entered into the hash table as there should
 * never be a need to bind them directly from a target names. If you want to
 * access an internal includes node by name, first access the actual target and
 * then read the internal includes node from there.
 */

static target_ptr get_target_includes( target_ptr const t )
{
    if ( !t->includes )
    {
        target_ptr const i = b2::jam::make_ptr<_target>();
        i->name = object_copy( t->name );
        i->boundname = object_copy( i->name );
        i->flags |= T_FLAG_NOTFILE | T_FLAG_INTERNAL;
        t->includes = i;
    }
    return t->includes;
}


/*
 * target_include() - adds a target to the given targe's 'included' list
 * target_include_many() - adds targets to the given target's 'included' list
 *
 * Included targets are modeled as dependencies of the including target's
 * internal include node.
 */

void target_include( target_ptr const including, target_ptr const included )
{
    target_ptr const internal = get_target_includes( including );
    targetentry( internal->depends, included );
}

void target_include_many( target_ptr const including, list_ptr const included_names
    )
{
    target_ptr const internal = get_target_includes( including );
    targetlist( internal->depends, included_names );
}


/*
 * enter_rule() - return pointer to RULE, creating it if necessary in
 * target_module.
 */

static rule_ptr enter_rule( object_ptr rulename, module_ptr target_module )
{
    int found;
    rule_ptr const r = (rule_ptr)hash_insert( demand_rules( target_module ),
        rulename, &found );
    if ( !found )
    {
        r->name = object_copy( rulename );
        r->procedure = 0;
        r->module = 0;
        r->actions = 0;
        r->exported = 0;
        r->module = target_module;
    }
    return r;
}


/*
 * define_rule() - return pointer to RULE, creating it if necessary in
 * target_module. Prepare it to accept a body or action originating in
 * src_module.
 */

static rule_ptr define_rule( module_ptr src_module, object_ptr rulename,
    module_ptr target_module )
{
    rule_ptr const r = enter_rule( rulename, target_module );
    if ( r->module != src_module )
    {
        /* If the rule was imported from elsewhere, clear it now. */
        set_rule_body( r, 0 );
        set_rule_actions( r, 0 );
        /* r will be executed in the source module. */
        r->module = src_module;
    }
    return r;
}


void rule_free( rule_ptr r )
{
    object_free( r->name );
    r->name = 0;
    if ( r->procedure )
        function_free( r->procedure );
    r->procedure = 0;
    if ( r->actions )
        actions_free( r->actions );
    r->actions = 0;
}


/*
 * bindtarget() - return pointer to TARGET, creating it if necessary.
 */

target_ptr bindtarget( object_ptr const target_name )
{
    int found;
    target_ptr t;

    if ( !targethash )
        targethash = hashinit( sizeof( TARGET ), "targets" );

    t = (target_ptr)hash_insert( targethash, target_name, &found );
    if ( !found )
    {
        b2::jam::ctor_ptr<_target>(t);
        t->name = object_copy( target_name );
        t->boundname = object_copy( t->name );  /* default for T_FLAG_NOTFILE */
    }

    return t;
}


static void bind_explicitly_located_target( target_ptr t, void * )
{
    if ( !( t->flags & T_FLAG_NOTFILE ) )
    {
        /* Check if there is a setting for LOCATE. */
        settings_ptr s = t->settings;
        for ( ; s ; s = s->next )
        {
            if ( object_equal( s->symbol, constant_LOCATE ) && ! list_empty( s->value ) )
            {
                set_explicit_binding( t->name, list_front( s->value ) );
                break;
            }
        }
    }
}


void bind_explicitly_located_targets()
{
    if ( targethash )
        hash_enumerate( targethash, bind_explicitly_located_target );
}


/*
 * touch_target() - mark a target to simulate being new.
 */

void touch_target( object_ptr const t )
{
    bindtarget( t )->flags |= T_FLAG_TOUCHED;
}


/*
 * target_scc() - returns the root of a strongly connected component that this
 * target is a part of.
 */

target_ptr target_scc( target_ptr t )
{
    target_ptr result = t;
    while ( result->scc_root )
        result = result->scc_root;
    while ( t->scc_root )
    {
        target_ptr const tmp = t->scc_root;
        t->scc_root = result;
        t = tmp;
    }
    return result;
}


/*
 * targetlist() - turn list of target names into a TARGET chain.
 *
 * Inputs:
 *  chain    existing TARGETS to append to
 *  targets  list of target names
 */

void targetlist( targets_uptr& chain, list_ptr target_names )
{
    LISTITER iter = list_begin( target_names );
    LISTITER const end = list_end( target_names );
    for ( ; iter != end; iter = list_next( iter ) )
        targetentry( chain, bindtarget( list_item( iter ) ) );
}


/*
 * targetentry() - add a TARGET to a chain of TARGETS.
 *
 * Inputs:
 *  chain   existing TARGETS to append to
 *  target  new target to append
 */

void targetentry( targets_uptr& chain, target_ptr target )
{
    auto c = b2::jam::make_unique_jptr<TARGETS>();
    c->target = target;

    targets_ptr tail = c.get();
    if ( !chain ) chain.reset(c.release());
    else chain->tail->next.reset(c.release());
    chain->tail = tail;
}


/*
 * targetchain() - append two TARGET chains.
 *
 * Inputs:
 *  chain   existing TARGETS to append to
 *  target  new target to append
 */

targets_uptr targetchain( targets_uptr chain, targets_uptr targets )
{
    if ( !targets ) return chain;
    if ( !chain ) return targets;

    targets_ptr tail = targets->tail;
    chain->tail->next = std::move(targets);
    chain->tail = tail;
    return chain;
}

/*
 * targets_pop() - removes the first TARGET from the chain.
 */

targets_uptr targets_pop(targets_uptr chain)
{
    targets_uptr result;
    if ( chain && chain->next )
    {
        chain->next->tail = chain->tail;
        result = std::move( chain->next );
    }
    return result;
}

/*
 * action_free - decrement the ACTIONs reference count and (maybe) free it.
 */

void action_free( action_ptr action )
{
    if ( --action->refs == 0 )
    {
        b2::jam::free_ptr(action);
    }
}


/*
 * actionlist() - append to an ACTION chain.
 */

actions_ptr actionlist( actions_ptr chain, action_ptr action )
{
    actions_ptr const actions = (actions_ptr)BJAM_MALLOC( sizeof( ACTIONS ) );
    actions->action = action;
    ++action->refs;
    if ( !chain ) chain = actions;
    else chain->tail->next = actions;
    chain->tail = actions;
    actions->next = 0;
    return chain;
}

static settings_ptr settings_freelist;


/*
 * addsettings() - add a deferred "set" command to a target.
 *
 * Adds a variable setting (varname=list) onto a chain of settings for a
 * particular target. 'flag' controls the relationship between new and old
 * values in the same way as in var_set() function (see variable.c). Returns the
 * head of the settings chain.
 */

settings_ptr addsettings( settings_ptr head, int flag, object_ptr symbol,
    list_ptr value )
{
    settings_ptr v;

    /* Look for previous settings. */
    for ( v = head; v; v = v->next )
        if ( object_equal( v->symbol, symbol ) )
            break;

    /* If not previously set, alloc a new. */
    /* If appending, do so. */
    /* Else free old and set new. */
    if ( !v )
    {
        v = settings_freelist;
        if ( v )
            settings_freelist = v->next;
        else
            v = (settings_ptr)BJAM_MALLOC( sizeof( *v ) );

        v->symbol = object_copy( symbol );
        v->value = value;
        v->next = head;
        head = v;
    }
    else if ( flag == VAR_APPEND )
    {
        v->value = list_append( v->value, value );
    }
    else if ( flag != VAR_DEFAULT )
    {
        list_free( v->value );
        v->value = value;
    }
    else
        list_free( value );

    /* Return (new) head of list. */
    return head;
}


/*
 * pushsettings() - set all target specific variables.
 */

void pushsettings( module_ptr module, settings_ptr v )
{
    for ( ; v; v = v->next )
        v->value = var_swap( module, v->symbol, v->value );
}


/*
 * popsettings() - reset target specific variables to their pre-push values.
 */

void popsettings( module_ptr module, settings_ptr v )
{
    pushsettings( module, v );  /* just swap again */
}


/*
 * copysettings() - duplicate a settings list, returning the new copy.
 */

settings_ptr copysettings( settings_ptr head )
{
    settings_ptr copy = 0;
    settings_ptr v;
    for ( v = head; v; v = v->next )
        copy = addsettings( copy, VAR_SET, v->symbol, list_copy( v->value ) );
    return copy;
}


/*
 * freeactions() - delete an action list.
 */

void freeactions( actions_ptr chain )
{
    while ( chain )
    {
        actions_ptr const n = chain->next;
        action_free( chain->action );
        BJAM_FREE( chain );
        chain = n;
    }
}


/*
 * freesettings() - delete a settings list.
 */

void freesettings( settings_ptr v )
{
    while ( v )
    {
        settings_ptr const n = v->next;
        object_free( v->symbol );
        list_free( v->value );
        v->next = settings_freelist;
        settings_freelist = v;
        v = n;
    }
}


static void freetarget( target_ptr const t, void * )
{
    if ( t->name       ) object_free ( t->name       );
    if ( t->boundname  ) object_free ( t->boundname  );
    if ( t->settings   ) freesettings( t->settings   );
    if ( t->depends    ) t->depends.reset();
    if ( t->dependants ) t->dependants.reset();
    if ( t->parents    ) t->parents.reset();
    if ( t->actions    ) freeactions ( t->actions    );
    if ( t->includes   )
    {
        freetarget( t->includes, (void *)0 );
        BJAM_FREE( t->includes );
    }
    t->~_target();
}


/*
 * rules_done() - free RULE and TARGET tables.
 */

void rules_done()
{
    if ( targethash )
    {
        hash_enumerate( targethash, freetarget );
        hashdone( targethash );
    }
    while ( settings_freelist )
    {
        settings_ptr const n = settings_freelist->next;
        BJAM_FREE( settings_freelist );
        settings_freelist = n;
    }
}


/*
 * actions_refer() - add a new reference to the given actions.
 */

void actions_refer( rule_actions_ptr a )
{
    ++a->reference_count;
}


/*
 * actions_free() - release a reference to given actions.
 */

void actions_free( rule_actions_ptr a )
{
    if ( --a->reference_count <= 0 )
    {
        function_free( a->command );
        list_free( a->bindlist );
        BJAM_FREE( a );
    }
}


/*
 * set_rule_body() - set the argument list and procedure of the given rule.
 */

static void set_rule_body( rule_ptr rule, function_ptr procedure )
{
    if ( procedure )
        function_refer( procedure );
    if ( rule->procedure )
        function_free( rule->procedure );
    rule->procedure = procedure;
}


/*
 * global_name() - given a rule, return the name for a corresponding rule in the
 * global module.
 */

static object_ptr global_rule_name( rule_ptr r )
{
    if ( r->module == root_module() )
        return object_copy( r->name );

    {
        char name[ 4096 ] = "";
        if ( r->module->name )
        {
            strncat( name, object_str( r->module->name ), sizeof( name ) - 1 );
            strncat( name, ".", sizeof( name ) - 1 );
        }
        strncat( name, object_str( r->name ), sizeof( name ) - 1 );
        return object_new( name );
    }
}


/*
 * global_rule() - given a rule, produce a corresponding entry in the global
 * module.
 */

static rule_ptr global_rule( rule_ptr r )
{
    if ( r->module == root_module() )
        return r;

    {
        object_ptr name = global_rule_name( r );
        rule_ptr const result = define_rule( r->module, name, root_module() );
        object_free( name );
        return result;
    }
}


/*
 * new_rule_body() - make a new rule named rulename in the given module, with
 * the given argument list and procedure. If exported is true, the rule is
 * exported to the global module as modulename.rulename.
 */

rule_ptr new_rule_body( module_ptr m, object_ptr rulename, function_ptr procedure,
    int exported )
{
    rule_ptr const local = define_rule( m, rulename, m );
    local->exported = exported;
    set_rule_body( local, procedure );

    /* Mark the procedure with the global rule name, regardless of whether the
     * rule is exported. That gives us something reasonably identifiable that we
     * can use, e.g. in profiling output. Only do this once, since this could be
     * called multiple times with the same procedure.
     */
    if ( !function_rulename( procedure ) )
        function_set_rulename( procedure, global_rule_name( local ) );

    return local;
}


static void set_rule_actions( rule_ptr rule, rule_actions_ptr actions )
{
    if ( actions )
        actions_refer( actions );
    if ( rule->actions )
        actions_free( rule->actions );
    rule->actions = actions;
}


static rule_actions_ptr actions_new( function_ptr command, list_ptr bindlist,
    int flags )
{
    rule_actions_ptr const result = (rule_actions_ptr)BJAM_MALLOC( sizeof(
        rule_actions ) );
    function_refer( command );
    result->command = command;
    result->bindlist = bindlist;
    result->flags = flags;
    result->reference_count = 0;
    return result;
}


rule_ptr new_rule_actions( module_ptr m, object_ptr rulename, function_ptr command,
    list_ptr bindlist, int flags )
{
    rule_ptr const local = define_rule( m, rulename, m );
    rule_ptr const global = global_rule( local );
    set_rule_actions( local, actions_new( command, bindlist, flags ) );
    set_rule_actions( global, local->actions );
    return local;
}


/*
 * Looks for a rule in the specified module, and returns it, if found. First
 * checks if the rule is present in the module's rule table. Second, if the
 * rule's name is in the form name1.name2 and name1 is in the list of imported
 * modules, look in module 'name1' for rule 'name2'.
 */

rule_ptr lookup_rule( object_ptr rulename, module_ptr m, int local_only )
{
    rule_ptr r;
    rule_ptr result = 0;
    module_ptr original_module = m;

    if ( m->class_module )
        m = m->class_module;

    if ( m->rules && ( r = (rule_ptr)hash_find( m->rules, rulename ) ) )
        result = r;
    else if ( !local_only && m->imported_modules )
    {
        /* Try splitting the name into module and rule. */
        const char * p = strchr( object_str( rulename ), '.' ) ;
        if ( p )
        {
            /* Now, r->name keeps the module name, and p + 1 keeps the rule
             * name.
             */
            object_ptr rule_part = object_new( p + 1 );
            object_ptr module_part;
            {
                string buf[ 1 ];
                string_new( buf );
                string_append_range( buf, object_str( rulename ), p );
                module_part = object_new( buf->value );
                string_free( buf );
            }
            if ( hash_find( m->imported_modules, module_part ) )
                result = lookup_rule( rule_part, bindmodule( module_part ), 1 );
            object_free( module_part );
            object_free( rule_part );
        }
    }

    if ( result )
    {
        if ( local_only && !result->exported )
            result = 0;
        else if ( original_module != m )
        {
            /* Lookup started in class module. We have found a rule in class
             * module, which is marked for execution in that module, or in some
             * instance. Mark it for execution in the instance where we started
             * the lookup.
             */
            int const execute_in_class = result->module == m;
            int const execute_in_some_instance =
                result->module->class_module == m;
            if ( execute_in_class || execute_in_some_instance )
                result->module = original_module;
        }
    }

    return result;
}


rule_ptr bindrule( object_ptr rulename, module_ptr m )
{
    rule_ptr result = lookup_rule( rulename, m, 0 );
    if ( !result )
        result = lookup_rule( rulename, root_module(), 0 );
    /* We have only one caller, 'evaluate_rule', which will complain about
     * calling an undefined rule. We could issue the error here, but we do not
     * have the necessary information, such as frame.
     */
    if ( !result )
        result = enter_rule( rulename, m );
    return result;
}


rule_ptr import_rule( rule_ptr source, module_ptr m, object_ptr name )
{
    rule_ptr const dest = define_rule( source->module, name, m );
    set_rule_body( dest, source->procedure );
    set_rule_actions( dest, source->actions );
    return dest;
}


void rule_localize( rule_ptr rule, module_ptr m )
{
    rule->module = m;
    if ( rule->procedure )
    {
        function_ptr procedure = function_unbind_variables( rule->procedure );
        function_refer( procedure );
        function_free( rule->procedure );
        rule->procedure = procedure;
    }
}
