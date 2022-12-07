///////////////////////////////////////////////////////////////
//  Copyright 2021 John Maddock. 
//  Copyright 2021 Matt Borland. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#define BOOST_MP_STANDALONE

#include "test_arithmetic.hpp"
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/logged_adaptor.hpp>

int main()
{
   test<boost::multiprecision::number<boost::multiprecision::logged_adaptor<typename boost::multiprecision::mpf_float::backend_type> > >();
   return boost::report_errors();
}
