/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2020 Jeff Trull. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

//O --c++17
//O -Werror

// Test __has_include() with some interesting cases

#if __has_include(/* comment */ __FILE__ /* comment */)
#define GOTTHISFILE_WITHCOMMENTS
#else
#warning cannot find using file macro when comments are present
#endif

//H 10: t_2_026.cpp(15): #if
//H 01: <built-in>(1): __FILE__
//H 02: "$F"
//H 03: "$F"
//H 11: t_2_026.cpp(15): #if __has_include(/* comment */ __FILE__ /* comment */): 1
//H 10: t_2_026.cpp(16): #define
//H 08: t_2_026.cpp(16): GOTTHISFILE_WITHCOMMENTS=
//H 10: t_2_026.cpp(17): #else


// Using __has_include() in a larger expression

#if __has_include("t_2_026.cpp") && 1
#define GOTFILE_ANDWITHTRUE
#else
#warning has_include of ourselves anded with 1 failed
#endif

#if __has_include("t_2_026.cpp") && 0
#warning has_include of ourselves anded with 0 should fail but did not
#else
#define NOFILE_ANDWITHFALSE
#endif

#if !__has_include("t_2_026.cpp") || 1
#define NOFILE_ORWITHTRUE
#else
#warning not has_include of ourselves ored with 1 failed
#endif

#if !__has_include("t_2_026.cpp") || 0
#warning not has_included of ourselves ored with 0 should fail but did not
#else
#define NOFILE_ORWITHFALSE
#endif

//H 10: t_2_026.cpp(33): #if
//H 11: t_2_026.cpp(33): #if __has_include("t_2_026.cpp") && 1: 1
//H 10: t_2_026.cpp(34): #define
//H 08: t_2_026.cpp(34): GOTFILE_ANDWITHTRUE=
//H 10: t_2_026.cpp(35): #else
//H 10: t_2_026.cpp(39): #if
//H 11: t_2_026.cpp(39): #if __has_include("t_2_026.cpp") && 0: 0
//H 10: t_2_026.cpp(42): #define
//H 08: t_2_026.cpp(42): NOFILE_ANDWITHFALSE=
//H 10: t_2_026.cpp(43): #endif
//H 10: t_2_026.cpp(45): #if
//H 11: t_2_026.cpp(45): #if !__has_include("t_2_026.cpp") || 1: 1
//H 10: t_2_026.cpp(46): #define
//H 08: t_2_026.cpp(46): NOFILE_ORWITHTRUE=
//H 10: t_2_026.cpp(47): #else
//H 10: t_2_026.cpp(51): #if
//H 11: t_2_026.cpp(51): #if !__has_include("t_2_026.cpp") || 0: 0
//H 10: t_2_026.cpp(54): #define
//H 08: t_2_026.cpp(54): NOFILE_ORWITHFALSE=
//H 10: t_2_026.cpp(55): #endif
