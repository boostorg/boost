///////////////////////////////////////////////////////////////
//  Copyright 2022 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifdef TEST_CPP_INT
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_complex.hpp>
#endif
#ifdef TEST_GMP
#include <boost/multiprecision/gmp.hpp>
#endif
#ifdef TEST_TOMMATH
#include <boost/multiprecision/tommath.hpp>
#endif
#ifdef TEST_FLOAT128
#include <boost/multiprecision/float128.hpp>
#include <boost/multiprecision/complex128.hpp>
#endif
#ifdef TEST_MPC
#include <boost/multiprecision/mpc.hpp>
#endif
#ifdef TEST_MPFI
#include <boost/multiprecision/mpfi.hpp>
#endif

#include <sstream>
#include "test.hpp"

template <class T>
void test_integer()
{
   std::istringstream iss("123#");
   T                  val;
   iss >> val;
   BOOST_CHECK_EQUAL(val, 123);
}

template <class T>
void test_rational()
{
   {
      std::istringstream iss("123#");
      T                  val;
      iss >> val;
      BOOST_CHECK_EQUAL(val, 123);
   }
   {
      std::istringstream iss("123/23#");
      T                  val;
      iss >> val;
      BOOST_CHECK_EQUAL(val, T(123, 23));
   }
}

template <class T>
void test_float()
{
   std::istringstream iss("123.0#");
   T                  val;
   iss >> val;
   BOOST_CHECK_EQUAL(val, 123.0);
}

template <class T>
void test_complex()
{
   {
      std::istringstream iss("123.0#");
      T                  val;
      iss >> val;
      BOOST_CHECK_EQUAL(val, 123.0);
   }
   {
      std::istringstream iss("(123.0,23.0)#");
      T                  val;
      iss >> val;
      BOOST_CHECK_EQUAL(val, T(123.0, 23.0));
   }
}

template <class T>
void test_interval()
{
   {
      std::istringstream iss("123.0#");
      T                  val;
      iss >> val;
      BOOST_CHECK_EQUAL(val, 123.0);
   }
   {
      std::istringstream iss("{123.0,124.0}#");
      T                  val;
      iss >> val;
      std::cout << val << std::endl;
      BOOST_CHECK_EQUAL(val, T(123.0, 124.0));
   }
}


int main()
{
#ifdef TEST_CPP_INT
   test_integer<boost::multiprecision::cpp_int>();
   test_rational<boost::multiprecision::cpp_rational>();
   test_float<boost::multiprecision::cpp_bin_float_50>();
   test_complex<boost::multiprecision::cpp_complex_50>();
#endif
#ifdef TEST_GMP
   test_integer<boost::multiprecision::mpz_int>();
   test_rational<boost::multiprecision::mpq_rational>();
   test_float<boost::multiprecision::mpf_float_50>();
#endif
#ifdef TEST_TOMMATH
   test_integer<boost::multiprecision::tom_int>();
   test_rational<boost::multiprecision::tom_rational>();
#endif
#ifdef TEST_FLOAT128
   test_float<boost::multiprecision::float128>();
   test_complex<boost::multiprecision::complex128>();
#endif
#ifdef TEST_MPC
   test_float<boost::multiprecision::mpfr_float_50>();
   test_complex<boost::multiprecision::mpc_complex_50>();
#endif
#ifdef TEST_MPFI
   test_interval<boost::multiprecision::mpfi_float_50>();
#endif
   return 0;
}
