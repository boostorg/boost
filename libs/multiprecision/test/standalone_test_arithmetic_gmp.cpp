///////////////////////////////////////////////////////////////
//  Copyright 2012 John Maddock. 
//  Copyright 2021 Matt Borland. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#define BOOST_MP_STANDALONE

#include "test_arithmetic.hpp"
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/rational_adaptor.hpp>

int main()
{
   test<boost::multiprecision::mpz_int>();
   return boost::report_errors();
}
