///////////////////////////////////////////////////////////////
//  Copyright 2012 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#define BOOST_CHRONO_HEADER_ONLY

#if !defined(TEST_MPZ) && !defined(TEST_TOMMATH) && !defined(TEST_CPP_INT)
#define TEST_MPZ
#define TEST_TOMMATH
#define TEST_CPP_INT
#endif

#ifdef TEST_MPZ
#include <boost/multiprecision/gmp.hpp>
#endif
#ifdef TEST_TOMMATH
#include <boost/multiprecision/tommath.hpp>
#endif
#ifdef TEST_CPP_INT
#include <boost/multiprecision/cpp_int.hpp>
#endif
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/chrono.hpp>
#include <boost/random.hpp>
#include <map>

template <class Clock>
struct stopwatch
{
   typedef typename Clock::duration duration;
   stopwatch()
   {
      m_start = Clock::now();
   }
   duration elapsed()
   {
      return Clock::now() - m_start;
   }
   void reset()
   {
      m_start = Clock::now();
   }

 private:
   typename Clock::time_point m_start;
};

extern unsigned allocation_count;

extern std::map<std::string, double> results;
extern double                        min_time;

template <class IntType>
boost::chrono::duration<double> test_miller_rabin(const char* name)
{
   using namespace boost::random;

   stopwatch<boost::chrono::high_resolution_clock> c;

   independent_bits_engine<mt11213b, 256, IntType> gen;
   //
   // We must use a different generator for the tests and number generation, otherwise
   // we get false positives.
   //
   mt19937  gen2;
   unsigned result_count = 0;

   for (unsigned i = 0; i < 1000; ++i)
   {
      IntType n = gen();
      if (boost::multiprecision::miller_rabin_test(n, 25, gen2))
         ++result_count;
   }
   boost::chrono::duration<double> t = c.elapsed();
   double                          d = t.count();
   if (d < min_time)
      min_time = d;
   results[name] = d;
   std::cout << "Time for " << std::setw(30) << std::left << name << " = " << d << std::endl;
   std::cout << "Number of primes found = " << result_count << std::endl;
   return t;
}

boost::chrono::duration<double> test_miller_rabin_gmp();

void test01();
void test02();
void test03();
void test04();
void test05();
void test06();
void test07();
void test08();
void test09();
void test10();
void test11();
void test12();
void test13();
void test14();
void test15();
void test16();
void test17();
void test18();
void test19();
