/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * make.h - bring a target up to date, once rules are in place
 */

#ifndef MAKE_SW20111118_H
#define MAKE_SW20111118_H

#include "config.h"
#include "lists.h"
#include "object.h"
#include "rules.h"

int32_t make( LIST * targets, int32_t anyhow );
int32_t make1( LIST * t );

typedef struct {
    int32_t temp;
    int32_t updating;
    int32_t cantfind;
    int32_t cantmake;
    int32_t targets;
    int32_t made;
} COUNTS ;


void make0( TARGET * t, TARGET * p, int32_t depth, COUNTS * counts, int32_t anyhow,
    TARGET * rescanning );


/* Specifies that the target should be updated. */
void mark_target_for_updating( OBJECT * target );

/* Returns targets previously passed to mark_target_for_updating(). */
LIST * targets_to_update();

/* Clears/unmarks all targets currently marked for update. */
void clear_targets_to_update();

#endif
