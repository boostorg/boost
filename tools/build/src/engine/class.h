/* Copyright Vladimir Prus 2003. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE.txt or copy at https://www.bfgroup.xyz/b2/LICENSE.txt) */

#ifndef CLASS_H_VP_2003_08_01
#define CLASS_H_VP_2003_08_01

#include "config.h"
#include "lists.h"
#include "frames.h"

OBJECT * make_class_module( LIST * xname, LIST * bases, FRAME * frame );
void class_done( void );

#endif
