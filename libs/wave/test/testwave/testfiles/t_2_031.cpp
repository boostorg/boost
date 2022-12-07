/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser.
    Copyright (c) 2022 Jeff Trull. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

//O --c++11

// test the error reporting for unknown directives in C++11 mode
// (newline at EOF not required)

//R
//E t_2_031.cpp(18): error: ill formed preprocessor directive: #this_is_a_unknown_pp_directive with some parameter
#this_is_a_unknown_pp_directive with some parameter

//H 21: t_2_031.cpp(18): #this_is_a_unknown_pp_directive with some parameter
//H
//H 18: boost::wave::preprocess_exception
