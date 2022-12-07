///////////////////////////////////////////////////////////////////////////////
//  Copyright 2018 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/multiprecision/gmp.hpp>

#include "eigen.hpp"

int main()
{
   test_float_type<boost::multiprecision::mpf_float_50>();
   test_float_type<boost::multiprecision::mpf_float>();
   return 0;
}
