#ifndef B2_CONFIG_H
#define B2_CONFIG_H

/*
Copyright 2002-2021 Rene Rivera.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#define OPT_HEADER_CACHE_EXT 1
#define OPT_GRAPH_DEBUG_EXT 1
#define OPT_SEMAPHORE 1
#define OPT_AT_FILES 1
#define OPT_DEBUG_PROFILE 1
#define JAM_DEBUGGER 1
#define OPT_FIX_TARGET_VARIABLES_EXT 1
#define OPT_IMPROVED_PATIENCE_EXT 1
// #define BJAM_NO_MEM_CACHE 1

// Autodetect various operating systems..

#if defined(_WIN32) || defined(_WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__)
    #define NT 1
#endif

#if defined(__VMS) || defined(__VMS_VER)
    #if !defined(VMS)
        #define VMS 1
    #endif
#endif

// To work around QEMU failures on mixed mode situations (32 vs 64) we need to
// enable partial LFS support in system headers. And we need to do this before
// any system headers are included.

#if !defined(NT) && !defined(VMS)
#   define _FILE_OFFSET_BITS 64
#endif

// Correct missing types in some earlier compilers..

#include <stdint.h>
#ifndef INT32_MIN

// VS 2013 is barely C++11/C99. And opts to not provide specific sized int types.
// Provide a generic implementation of the sizes we use.
#if UINT_MAX == 0xffffffff
typedef int int32_t;
#elif (USHRT_MAX == 0xffffffff)
typedef short int32_t;
#elif ULONG_MAX == 0xffffffff
typedef long int32_t;
#endif

#endif

#endif
