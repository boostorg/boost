///////////////////////////////////////////////////////////////
//  Copyright 2021 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt
//
// Simple test that cpp_int -> double conversion has less than 0.5ulp error
// and rounds to even in case of ties.
// See https://github.com/boostorg/multiprecision/issues/360.
//
#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include "test.hpp"

using namespace boost::multiprecision;

#ifdef BOOST_MSVC
#pragma warning(disable : 4127)
#endif

template <class T>
T generate_random(unsigned bits_wanted)
{
   static boost::random::mt19937               gen;
   typedef boost::random::mt19937::result_type random_type;

   T        max_val;
   unsigned digits;
   if (std::numeric_limits<T>::is_bounded && (bits_wanted == (unsigned)std::numeric_limits<T>::digits))
   {
      max_val = (std::numeric_limits<T>::max)();
      digits  = std::numeric_limits<T>::digits;
   }
   else
   {
      max_val = T(1) << bits_wanted;
      digits  = bits_wanted;
   }

   unsigned bits_per_r_val = std::numeric_limits<random_type>::digits - 1;
   while ((random_type(1) << bits_per_r_val) > (gen.max)())
      --bits_per_r_val;

   unsigned terms_needed = digits / bits_per_r_val + 1;

   T val = 0;
   for (unsigned i = 0; i < terms_needed; ++i)
   {
      val *= (gen.max)();
      val += gen();
   }
   val %= max_val;
   return val;
}

template <class From, class To>
void test_convert()
{
   boost::random::mt19937 gen;
   boost::random::uniform_int_distribution<> d(20, (std::min)(200, std::numeric_limits<To>::max_exponent - 2));

   for (unsigned i = 0; i < 10000; ++i)
   {
      int  bits = d(gen);
      From from = generate_random<From>(bits);
      To   t1(from);
      From b(t1);
      std::size_t m = msb(from);
      if (m >= std::numeric_limits<To>::digits)
      {
         // For error <= 1ulp
         // Note msb(from) returns one less than the number of bits in from:
         From max_error = (From(1) << (m - std::numeric_limits<To>::digits));
         BOOST_TEST_GE(max_error, abs(b - from));
         if (max_error < abs(b - from))
            // debugging help:
            std::cout << from << std::endl
                      << b << std::endl
                      << abs(b - from) << std::endl;
         if (max_error == abs(b - from))
         {
            // Check we rounded to even in case of tie:
            BOOST_TEST_GT(lsb(b), (1 + msb(from) - std::numeric_limits<To>::digits));
            if (lsb(b) <= (1 + msb(from) - std::numeric_limits<To>::digits))
            {
               // debugging help:
               std::cout << from << std::endl
                         << b << std::endl
                         << abs(b - from) << std::endl;
            }
         }
      }
      else
      {
         BOOST_TEST_EQ(b, from);
      }
   }
}

int main()
{
   test_convert<cpp_int, float>();
   test_convert<cpp_int, double>();
   return boost::report_errors();
}
