///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "sf_performance.hpp"

void bessel_tests_01();
void bessel_tests_02();
void bessel_tests_03();
void bessel_tests_04();
void bessel_tests_05();
void bessel_tests_06();
void bessel_tests_07();
void bessel_tests_08();
void bessel_tests_09();
void bessel_tests_10();
void bessel_tests_11();
void bessel_tests_12();
void bessel_tests_13();
void bessel_tests_14();
void bessel_tests_15();
void bessel_tests_16();
void bessel_tests_17();
void bessel_tests_18();
void bessel_tests_19();

void bessel_tests()
{
   //
   // 50 digits first:
   //
   std::cout << "Testing Bessel Functions at 50 digits....." << std::endl;
#if defined(TEST_MPFR) || defined(TEST_MPFR_CLASS)
   mpfr_set_default_prec(50 * 1000L / 301L);
#endif
#ifdef TEST_MPREAL
   mpfr::mpreal::set_default_prec(50 * 1000L / 301L);
#endif

   bessel_tests_01();
   bessel_tests_02();
   bessel_tests_03();
   bessel_tests_04();
   bessel_tests_05();
   bessel_tests_06();
   bessel_tests_07();
   bessel_tests_08();
   bessel_tests_09();
   bessel_tests_10();

   //
   // Then 100 digits:
   //
   std::cout << "Testing Bessel Functions at 100 digits....." << std::endl;
#if defined(TEST_MPFR) || defined(TEST_MPFR_CLASS)
   mpfr_set_default_prec(100 * 1000L / 301L);
#endif
   bessel_tests_11();
   bessel_tests_12();
   bessel_tests_13();
   bessel_tests_14();
   bessel_tests_15();
   bessel_tests_16();
   bessel_tests_17();
   bessel_tests_18();
   bessel_tests_19();
}
