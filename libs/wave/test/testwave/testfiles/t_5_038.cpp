/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2022 Jeff Trull. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

=============================================================================*/

// Verify that long integer literals from a macro expansion parse correctly
// This test covers #162

//O --c++11
//O -DFOO=0x1234567ULL
#define BAZ (FOO*2UL+1UL)
#define BAR (BAZ + 1ULL)

#if defined(BAR) && (BAR == 0x2468AD0)
struct Bar {};
#else
#endif

//R #line 20 "t_5_038.cpp"
//R struct Bar {};
