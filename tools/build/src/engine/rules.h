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
 * rules.h -  targets, rules, and related information
 *
 * This file describes the structures holding the targets, rules, and related
 * information accumulated by interpreting the statements of the jam files.
 *
 * The following are defined:
 *
 *  RULE - a generic jam rule, the product of RULE and ACTIONS.
 *  ACTIONS - a chain of ACTIONs.
 *  ACTION - a RULE instance with targets and sources.
 *  SETTINGS - variables to set when executing a TARGET's ACTIONS.
 *  TARGETS - a chain of TARGETs.
 *  TARGET - an entity (e.g. a file) that can be built.
 */

#ifndef RULES_DWA_20011020_H
#define RULES_DWA_20011020_H

#include "config.h"
#include "function.h"
#include "mem.h"
#include "modules.h"
#include "timestamp.h"
#include <utility>

typedef struct _rule RULE;
typedef struct _target TARGET;
typedef struct _targets TARGETS;
typedef struct _action ACTION;
typedef struct _actions ACTIONS;
typedef struct _settings SETTINGS;

typedef RULE* rule_ptr;
typedef TARGET* target_ptr;
typedef TARGETS* targets_ptr;
typedef ACTION* action_ptr;
typedef ACTIONS* actions_ptr;
typedef SETTINGS* settings_ptr;

typedef RULE& rule_ref;
typedef TARGET& target_ref;
typedef TARGETS& targets_ref;
typedef ACTION& action_ref;
typedef ACTIONS& actions_ref;
typedef SETTINGS& settings_ref;

using rule_uptr = b2::jam::unique_jptr<_rule>;
using target_uptr = b2::jam::unique_jptr<_target>;
using targets_uptr = b2::jam::unique_jptr<_targets>;
using action_uptr = b2::jam::unique_jptr<_action>;
using actions_uptr = b2::jam::unique_jptr<_actions>;
using settings_uptr = b2::jam::unique_jptr<_settings>;

/* RULE - a generic jam rule, the product of RULE and ACTIONS. */

/* Build actions corresponding to a rule. */
struct rule_actions {
    int reference_count;
    function_ptr command; /* command string from ACTIONS */
    list_ptr bindlist;
    int flags; /* modifiers on ACTIONS */
};

#define RULE_NEWSRCS 0x01 /* $(>) is updated sources only */
#define RULE_TOGETHER 0x02 /* combine actions on single target */
#define RULE_IGNORE 0x04 /* ignore return status of executes */
#define RULE_QUIETLY 0x08 /* do not mention it unless verbose */
#define RULE_PIECEMEAL 0x10 /* split exec so each $(>) is small */
#define RULE_EXISTING 0x20 /* $(>) is pre-existing sources only */

typedef struct rule_actions* rule_actions_ptr;

struct _rule {
    object_ptr name;
    function_ptr procedure;
    rule_actions_ptr actions; /* build actions, or NULL for no actions */
    module_ptr module; /* module in which this rule is executed */
    int exported; /* nonzero if this rule is supposed to appear in
                   * the global module and be automatically
                   * imported into other modules
                   */
};

/* ACTIONS - a chain of ACTIONs. */
struct _actions {
    actions_ptr next;
    actions_ptr tail; /* valid only for head */
    action_ptr action;
};

/* ACTION - a RULE instance with targets and sources. */
struct _action {
    rule_ptr rule;
    targets_uptr targets;
    targets_uptr sources; /* aka $(>) */
    char running; /* has been started */
#define A_INIT 0
#define A_RUNNING_NOEXEC 1
#define A_RUNNING 2
    int refs;

    /* WARNING: These variables are used to pass state required by make1cmds and
     * are not valid anywhere else.
     */
    void* first_cmd; /* Pointer to the first CMD created by this action */
    void* last_cmd; /* Pointer to the last CMD created by this action */
};

/* SETTINGS - variables to set when executing a TARGET's ACTIONS. */
struct _settings {
    settings_ptr next;
    object_ptr symbol; /* symbol name for var_set() */
    list_ptr value; /* symbol value for var_set() */
};

/* TARGETS - a chain of TARGETs. */
struct _targets {
    targets_uptr next = nullptr;
    targets_ptr tail = nullptr; /* valid only for head */
    target_ptr target = nullptr;

    ~_targets()
    {
        targets_uptr sink = std::move(next);
        while ( sink ) sink = std::move(sink->next);
    }
};

/* TARGET - an entity (e.g. a file) that can be built. */
struct _target {
    object_ptr name;
    object_ptr boundname; /* if search() relocates target */
    actions_ptr actions; /* rules to execute, if any */
    settings_ptr settings; /* variables to define */

    targets_uptr depends; /* dependencies */
    targets_uptr dependants; /* the inverse of dependencies */
    targets_uptr rebuilds; /* targets that should be force-rebuilt
                            * whenever this one is
                            */
    target_ptr includes; /* internal includes node */

    timestamp time; /* update time */
    timestamp leaf; /* update time of leaf sources */

    short flags; /* status info */

#define T_FLAG_TEMP 0x0001 /* TEMPORARY applied */
#define T_FLAG_NOCARE 0x0002 /* NOCARE applied */
#define T_FLAG_NOTFILE 0x0004 /* NOTFILE applied */
#define T_FLAG_TOUCHED 0x0008 /* ALWAYS applied or -t target */
#define T_FLAG_LEAVES 0x0010 /* LEAVES applied */
#define T_FLAG_NOUPDATE 0x0020 /* NOUPDATE applied */
#define T_FLAG_VISITED 0x0040 /* CWM: Used in debugging */

/* This flag has been added to support a new built-in rule named "RMBAD". It is
 * used to force removal of outdated targets whose dependencies fail to build.
 */
#define T_FLAG_RMOLD 0x0080 /* RMBAD applied */

/* This flag was added to support a new built-in rule named "FAIL_EXPECTED" used
 * to indicate that the result of running a given action should be inverted,
 * i.e. ok <=> fail. Useful for launching certain test runs from a Jamfile.
 */
#define T_FLAG_FAIL_EXPECTED 0x0100 /* FAIL_EXPECTED applied */

#define T_FLAG_INTERNAL 0x0200 /* internal INCLUDES node */

/* Indicates that the target must be a file. Prevents matching non-files, like
 * directories, when a target is searched.
 */
#define T_FLAG_ISFILE 0x0400

#define T_FLAG_PRECIOUS 0x0800

    char binding; /* how target relates to a real file or
                   * folder
                   */

#define T_BIND_UNBOUND 0 /* a disembodied name */
#define T_BIND_MISSING 1 /* could not find real file */
#define T_BIND_PARENTS 2 /* using parent's timestamp */
#define T_BIND_EXISTS 3 /* real file, timestamp valid */

    char fate; /* make0()'s diagnosis */

#define T_FATE_INIT 0 /* nothing done to target */
#define T_FATE_MAKING 1 /* make0(target) on stack */

#define T_FATE_STABLE 2 /* target did not need updating */
#define T_FATE_NEWER 3 /* target newer than parent */

#define T_FATE_SPOIL 4 /* >= SPOIL rebuilds parents */
#define T_FATE_ISTMP 4 /* unneeded temp target oddly present */

#define T_FATE_BUILD 5 /* >= BUILD rebuilds target */
#define T_FATE_TOUCHED 5 /* manually touched with -t */
#define T_FATE_REBUILD 6
#define T_FATE_MISSING 7 /* is missing, needs updating */
#define T_FATE_NEEDTMP 8 /* missing temp that must be rebuild */
#define T_FATE_OUTDATED 9 /* is out of date, needs updating */
#define T_FATE_UPDATE 10 /* deps updated, needs updating */

#define T_FATE_BROKEN 11 /* >= BROKEN ruins parents */
#define T_FATE_CANTFIND 11 /* no rules to make missing target */
#define T_FATE_CANTMAKE 12 /* can not find dependencies */

    char progress; /* tracks make1() progress */

#define T_MAKE_INIT 0 /* make1(target) not yet called */
#define T_MAKE_ONSTACK 1 /* make1(target) on stack */
#define T_MAKE_ACTIVE 2 /* make1(target) in make1b() */
#define T_MAKE_RUNNING 3 /* make1(target) running commands */
#define T_MAKE_DONE 4 /* make1(target) done */
#define T_MAKE_NOEXEC_DONE 5 /* make1(target) done with -n in effect */

#ifdef OPT_SEMAPHORE
#define T_MAKE_SEMAPHORE 5 /* Special target type for semaphores */
#endif

    char status; /* exec_cmd() result */

#ifdef OPT_SEMAPHORE
    target_ptr semaphore; /* used in serialization */
#endif

    int asynccnt; /* child deps outstanding */
    targets_uptr parents; /* used by make1() for completion */
    target_ptr scc_root; /* used by make to resolve cyclic includes
                          */
    target_ptr rescanning; /* used by make0 to mark visited targets
                            * when rescanning
                            */
    int depth; /* The depth of the target in the make0
                * stack.
                */
    char* cmds; /* type-punned command list */

    char const* failed;
};

/* Action related functions. */
void action_free(action_ptr);
actions_ptr actionlist(actions_ptr, action_ptr);
void freeactions(actions_ptr);
settings_ptr addsettings(settings_ptr, int flag, object_ptr symbol, list_ptr value);
void pushsettings(module_ptr, settings_ptr);
void popsettings(module_ptr, settings_ptr);
settings_ptr copysettings(settings_ptr);
void freesettings(settings_ptr);
void actions_refer(rule_actions_ptr);
void actions_free(rule_actions_ptr);

/* Rule related functions. */
rule_ptr bindrule(object_ptr rulename, module_ptr);
rule_ptr import_rule(rule_ptr source, module_ptr, object_ptr name);
void rule_localize(rule_ptr rule, module_ptr module);
rule_ptr new_rule_body(module_ptr, object_ptr rulename, function_ptr func, int exprt);
rule_ptr new_rule_actions(module_ptr, object_ptr rulename, function_ptr command, list_ptr bindlist, int flags);
void rule_free(rule_ptr);

/* Target related functions. */
void bind_explicitly_located_targets();
target_ptr bindtarget(object_ptr const);
targets_uptr targetchain(targets_uptr, targets_uptr);
void targetentry(targets_uptr&, target_ptr);
void target_include(target_ptr const including,
    target_ptr const included);
void target_include_many(target_ptr const including,
    list_ptr const included_names);
void targetlist(targets_uptr&, list_ptr target_names);
void touch_target(object_ptr const);
void clear_includes(target_ptr);
target_ptr target_scc(target_ptr);
targets_uptr targets_pop(targets_uptr);

/* Final module cleanup. */
void rules_done();

#endif
