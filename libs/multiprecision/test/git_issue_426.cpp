///////////////////////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/multiprecision/cpp_bin_float.hpp>
#ifdef TEST_MPFR
#include <boost/multiprecision/mpfr.hpp>
#endif
#ifdef TEST_FLOAT128
#include <boost/multiprecision/float128.hpp>
#endif
#include "test.hpp"

template <class T>
void test()
{
   T d = 360;
   for (int i = 2; i >= -2; --i)
   {
      T x = i * d;
      T y = remainder(x, d);
      if (y == 0)
         BOOST_CHECK_EQUAL(signbit(y), signbit(x));
      if (i == 0)
      {
         x = -x;
         y = remainder(x, d);
         if (y == 0)
            BOOST_CHECK_EQUAL(signbit(y), signbit(x));
      }
   }
}

int main()
{
   test<boost::multiprecision::cpp_bin_float_50>();
   // No signed zero:
   //test<boost::multiprecision::cpp_dec_float_50>();
   //test<boost::multiprecision::mpf_float_50>();
#ifdef TEST_MPFR
   test<boost::multiprecision::mpfr_float_50>();
#endif
#ifdef TEST_FLOAT128
   test<boost::multiprecision::float128>();
#endif
   return boost::report_errors();
}


