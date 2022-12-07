/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2022 Jeff Trull. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

=============================================================================*/

// Check generated line directives
// This test covers issue #24

// define one macro
#define FOO

// try a variation matrix:
// 1. initial #if true vs. false
// 2. with/without else clause
// 3. additional text immediately after #endif vs. one line later

#ifdef FOO
struct Bar {};
#endif
struct Quux {};

// initial #if false

#ifndef FOO
struct Bar {};
#endif
struct Quux {};

// adding else clause

#ifdef FOO
struct Bar {};
#else
struct Baz {};
#endif
struct Quux {};

#ifndef FOO
struct Bar {};
#else
struct Baz {};
#endif
struct Quux {};

// intermediate space before next line

#ifdef FOO
struct Bar {};
#endif

struct Quux {};

#ifndef FOO
struct Bar {};
#endif

struct Quux {};

#ifdef FOO
struct Bar {};
#else
struct Baz {};
#endif

struct Quux {};

#ifndef FOO
struct Bar {};
#else
struct Baz {};
#endif

struct Quux {};

// varying the location of the "true" conditional block among 3

#ifdef FOO
// first block is present
struct Bar {};
#elif defined(BAR)
struct Baz {};
#else
struct Boozle {};
#endif

struct Quux {};

#ifndef FOO
struct Bar {};
#elif !defined(BAR)
// second block is present
struct Baz {};
#else
struct Boozle {};
#endif

struct Quux {};

#ifndef FOO
struct Bar {};
#elif defined(BAR)
struct Baz {};
#else
// third block is present
struct Boozle {};
#endif

struct Quux {};

// test two-part conditionals that have an elif instead of an else
#ifdef FOO
// first part true
struct Bar {};
#elif defined(BAR)
struct Baz {};
#endif
struct Quux {};

#ifndef FOO
struct Bar {};
#elif !defined(BAR)
// second part true
struct Baz {};
#endif

struct Quux {};

// neither part true
#ifndef FOO
struct Bar {};
#elif defined(BAR)
struct Baz {};
#endif
struct Quux {};

// one more test: extra empty line before else clause
#ifndef FOO
struct Bar {};
#else

struct Baz {};
#endif
struct Quux {};



//R #line 23 "t_5_039.cpp"
//R struct Bar {};
//R 
//R struct Quux {};
//R #line 32 "t_5_039.cpp"
//R struct Quux {};
//R #line 37 "t_5_039.cpp"
//R struct Bar {};
//R #line 41 "t_5_039.cpp"
//R struct Quux {};
//R #line 46 "t_5_039.cpp"
//R struct Baz {};
//R 
//R struct Quux {};
//R #line 53 "t_5_039.cpp"
//R struct Bar {};
//R #line 56 "t_5_039.cpp"
//R struct Quux {};
//R #line 62 "t_5_039.cpp"
//R struct Quux {};
//R #line 65 "t_5_039.cpp"
//R struct Bar {};
//R #line 70 "t_5_039.cpp"
//R struct Quux {};
//R #line 75 "t_5_039.cpp"
//R struct Baz {};
//R #line 78 "t_5_039.cpp"
//R struct Quux {};
//R #line 84 "t_5_039.cpp"
//R struct Bar {};
//R #line 91 "t_5_039.cpp"
//R struct Quux {};
//R #line 97 "t_5_039.cpp"
//R struct Baz {};
//R #line 102 "t_5_039.cpp"
//R struct Quux {};
//R #line 110 "t_5_039.cpp"
//R struct Boozle {};
//R #line 113 "t_5_039.cpp"
//R struct Quux {};
//R #line 118 "t_5_039.cpp"
//R struct Bar {};
//R #line 122 "t_5_039.cpp"
//R struct Quux {};
//R #line 128 "t_5_039.cpp"
//R struct Baz {};
//R #line 131 "t_5_039.cpp"
//R struct Quux {};
//R #line 139 "t_5_039.cpp"
//R struct Quux {};
//R #line 146 "t_5_039.cpp"
//R struct Baz {};
//R 
//R struct Quux {};
