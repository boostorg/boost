/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * variable.h - handle jam multi-element variables
 */

#ifndef VARIABLE_SW20111119_H
#define VARIABLE_SW20111119_H

#include "config.h"
#include "lists.h"
#include "object.h"
#include "modules.h"

#include <string>


struct module_t;

void   var_defines( struct module_t *, const char * const * e, int preprocess );
LIST * var_get( struct module_t *, OBJECT * symbol );
void   var_set( struct module_t *, OBJECT * symbol, LIST * value, int flag );
LIST * var_swap( struct module_t *, OBJECT * symbol, LIST * value );
void   var_done( struct module_t * );

/*
 * Defines for var_set().
 */

#define VAR_SET      0   /* override previous value */
#define VAR_APPEND   1   /* append to previous value */
#define VAR_DEFAULT  2   /* set only if no previous value */

namespace b2 { namespace jam {
    struct variable
    {
        inline variable(const variable &v)
            : var_module(v.var_module), var_symbol(object_copy(v.var_symbol)) {}
        inline variable(struct module_t *m, const char *v)
            : var_module(m), var_symbol(object_new(v)) {}
        inline variable(const char *m, const char *v)
        {
            OBJECT * module_sym = object_new(m);
            var_module = bindmodule(module_sym);
            var_symbol = object_new(v);
            object_free(module_sym);
        }
        inline variable(const std::string &m, const std::string &v)
            : variable(m.c_str(), v.c_str()) {}
        inline explicit variable(const char *v) : variable(root_module(), v) {}
        inline explicit variable(const std::string &v) : variable(v.c_str()) {}
        inline ~variable()
        {
            if (var_symbol) object_free(var_symbol);
        }

        inline operator list() const { return list{var_get(var_module, var_symbol)}; }

        inline variable & operator=(list && v)
        {
            var_set(var_module, var_symbol, v.release(), VAR_SET);
            return *this;
        }
        inline variable & operator=(const list & v) { return *this = list{v}; }
        inline variable & operator=(const char *v) { return *this = list{object{v}}; }
        inline variable & operator=(const std::string &v) { return *this = list{object{v}}; }

        inline variable & operator+=(list & v)
        {
            var_set(var_module, var_symbol, v.release(), VAR_APPEND);
            return *this;
        }
        inline variable & operator+=(list && v)
        {
            var_set(var_module, var_symbol, v.release(), VAR_APPEND);
            return *this;
        }
        inline variable & operator+=(const char *v) { return *this += list{object{v}}; }
        inline variable & operator+=(const std::string &v) { return *this += list{object{v}}; }

        inline variable & operator|=(list & v)
        {
            var_set(var_module, var_symbol, v.release(), VAR_DEFAULT);
            return *this;
        }
        inline variable & operator|=(list && v)
        {
            var_set(var_module, var_symbol, v.release(), VAR_DEFAULT);
            return *this;
        }
        inline variable & operator|=(const char *v) { return *this |= list{object{v}}; }
        inline variable & operator|=(const std::string &v) { return *this |= list{object{v}}; }

        inline operator bool() const
        {
            LIST * l = var_get(var_module, var_symbol);
            return (!list_empty(l)) && (list_length(l) > 0);
        }

        private:

        struct module_t * var_module = nullptr;
        OBJECT * var_symbol = nullptr;
    };
}}

#endif
