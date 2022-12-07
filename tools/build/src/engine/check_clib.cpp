/*  Copyright 2021 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

/*
This program is a compile test for support of clib use.
This is used by the build script to guess and check the compiler to build the engine with.
*/

// Some headers we depend on..
#include <string.h>


int check_clib()
{
    { auto _ = strdup("-"); }
	return 0;
}
