///////////////////////////////////////////////////////////////
//  Copyright 2021 John Maddock.
//  Copyright 2022 Christopher Kormanyos.
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

//
// Compare arithmetic results using fixed_int to GMP results.
//

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#include <random>

#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "timer.hpp"
#include "test.hpp"

#ifdef _MSC_VER
#pragma warning(disable : 4127) //  Conditional expression is constant
#endif

#if !defined(TEST1) && !defined(TEST2) && !defined(TEST3) && !defined(TEST4) && !defined(TEST5) && !defined(TEST6)
#define TEST1
#define TEST2
#define TEST3
#define TEST4
#define TEST5
#define TEST6
#endif

namespace local_random
{

using generator_type = std::mt19937;
using random_type    = typename generator_type::result_type;

generator_type& my_generator()
{
  static generator_type generator_instance;

  return generator_instance;
}

template <class T>
T generate_random(unsigned bits_wanted)
{
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
   while ((random_type(1) << bits_per_r_val) > (my_generator().max)())
      --bits_per_r_val;

   unsigned terms_needed = digits / bits_per_r_val + 1;

   T val = 0;
   for (unsigned i = 0; i < terms_needed; ++i)
   {
      val *= (my_generator().max)();
      val += my_generator()();
   }
   val %= max_val;
   return val;
}

}

template <class Number>
struct tester
{
   using test_type  = Number;
   using checked    = typename test_type::backend_type::checked_type;
   using timer_type = boost::multiprecision::test_detail::timer_template<int, std::chrono::high_resolution_clock>;

   unsigned   last_error_count;
   timer_type tim;

   boost::multiprecision::mpz_int a, b, c, d;
   int                            si;
   unsigned                       ui;
   boost::multiprecision::double_limb_type large_ui;
   test_type                      a1, b1, c1, d1;

   static constexpr int double_limb_type_digit_counter()
   {
      return static_cast<int>(sizeof(boost::multiprecision::double_limb_type)) * 8;
   }

   void t1()
   {
      //
      // Arithmetic, non-mixed:
      //
      boost::multiprecision::mpq_rational x(a, b), y(c, d), z;
      boost::multiprecision::cpp_rational x1(a1, b1), y1(c1, d1), z1;

      BOOST_CHECK_EQUAL(x.str(), x1.str());
      BOOST_CHECK_EQUAL(y.str(), y1.str());

      // positive x, y:
      z = x + y;
      z1 = x1 + y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x - y;
      z1 = x1 - y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x * y;
      z1 = x1 * y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x / y;
      z1 = x1 / y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      
      // negative y:
      y = -y;
      y1 = -y1;
      BOOST_CHECK(y < 0);
      z  = x + y;
      z1 = x1 + y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - y;
      z1 = x1 - y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * y;
      z1 = x1 * y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / y;
      z1 = x1 / y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      
      // negative x:
      x.swap(y);
      x1.swap(y1);
      BOOST_CHECK(x < 0);
      z  = x + y;
      z1 = x1 + y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - y;
      z1 = x1 - y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * y;
      z1 = x1 * y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / y;
      z1 = x1 / y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // negative x, y:
      y = -y;
      y1 = -y1;
      BOOST_CHECK(x < 0);
      BOOST_CHECK(y < 0);
      z  = x + y;
      z1 = x1 + y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - y;
      z1 = x1 - y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * y;
      z1 = x1 * y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / y;
      z1 = x1 / y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // Inplace, negative x, y:
      BOOST_CHECK(x < 0);
      BOOST_CHECK(y < 0);
      z = x;
      z += y;
      z1 = x1;
      z1 += y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z -= y;
      z1 = x1;
      z1 -= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= y;
      z1 = x1;
      z1 *= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= z;
      z1 = x1;
      z1 *= z1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z /= y;
      z1 = x1;
      z1 /= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // Inplace, negative x:
      y  = -y;
      y1 = -y1;
      BOOST_CHECK(x < 0);
      z = x;
      z += y;
      z1 = x1;
      z1 += y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z -= y;
      z1 = x1;
      z1 -= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= y;
      z1 = x1;
      z1 *= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= z;
      z1 = x1;
      z1 *= z1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z /= y;
      z1 = x1;
      z1 /= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // Inplace, negative y:
      x.swap(y);
      x1.swap(y1);
      BOOST_CHECK(y < 0);
      z = x;
      z += y;
      z1 = x1;
      z1 += y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z -= y;
      z1 = x1;
      z1 -= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= y;
      z1 = x1;
      z1 *= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= z;
      z1 = x1;
      z1 *= z1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z /= y;
      z1 = x1;
      z1 /= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // Inplace, positive x, y:
      y = -y;
      y1 = -y1;
      BOOST_CHECK(x > 0);
      BOOST_CHECK(y > 0);
      z = x;
      z += y;
      z1 = x1;
      z1 += y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z -= y;
      z1 = x1;
      z1 -= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= y;
      z1 = x1;
      z1 *= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z *= z;
      z1 = x1;
      z1 *= z1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z /= y;
      z1 = x1;
      z1 /= y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      BOOST_CHECK_EQUAL((x == y), (x1 == y1));
      BOOST_CHECK_EQUAL((x != y), (x1 != y1));
      BOOST_CHECK_EQUAL((x <= y), (x1 <= y1));
      BOOST_CHECK_EQUAL((x >= y), (x1 >= y1));
      BOOST_CHECK_EQUAL((x < y), (x1 < y1));
      BOOST_CHECK_EQUAL((x > y), (x1 > y1));

      z = x;
      z1 = x1;
      BOOST_CHECK_EQUAL((x == z), (x1 == z1));
      BOOST_CHECK_EQUAL((x != z), (x1 != z1));
      BOOST_CHECK_EQUAL((x <= z), (x1 <= z1));
      BOOST_CHECK_EQUAL((x >= z), (x1 >= z1));
      BOOST_CHECK_EQUAL((x < z), (x1 < z1));
      BOOST_CHECK_EQUAL((x > z), (x1 > z1));
   }

   void t2()
   {
      //
      // Mixed with signed integer:
      //
      boost::multiprecision::mpq_rational x(a, b), y(c, d), z;
      boost::multiprecision::cpp_rational x1(a1, b1), y1(c1, d1), z1;

      BOOST_CHECK_EQUAL(x.str(), x1.str());
      BOOST_CHECK_EQUAL(y.str(), y1.str());

      // Both positive:
      z = x + si;
      z1 = x1 + si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x - si;
      z1 = x1 - si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x * si;
      z1 = x1 * si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x / si;
      z1 = x1 / si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si + x;
      z1 = si + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si - x;
      z1 = si - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si * x;
      z1 = si * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si / x;
      z1 = si / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      x = -x;
      x1 = -x1;
      z  = x + si;
      z1 = x1 + si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - si;
      z1 = x1 - si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * si;
      z1 = x1 * si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / si;
      z1 = x1 / si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si + x;
      z1 = si + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si - x;
      z1 = si - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si * x;
      z1 = si * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si / x;
      z1 = si / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x and si both negative:
      z  = x + si;
      z1 = x1 + si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - si;
      z1 = x1 - si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * si;
      z1 = x1 * si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / si;
      z1 = x1 / si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si + x;
      z1 = si + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si - x;
      z1 = si - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si * x;
      z1 = si * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si / x;
      z1 = si / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // si negative:
      x = -x;
      x1 = -x1;
      z  = x + si;
      z1 = x1 + si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - si;
      z1 = x1 - si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * si;
      z1 = x1 * si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / si;
      z1 = x1 / si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si + x;
      z1 = si + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si - x;
      z1 = si - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si * x;
      z1 = si * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = si / x;
      z1 = si / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      si = -si;
      // Inplace:
      z = x;
      z1 = x1;
      z += si;
      z1 += si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = x;
      z1 = x1;
      z -= si;
      z1 -= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= si;
      z1 *= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= si;
      z1 /= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // si negative:
      si = -si;
      z  = x;
      z1 = x1;
      z += si;
      z1 += si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= si;
      z1 -= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= si;
      z1 *= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= si;
      z1 /= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // Both negative:
      x  = -x;
      x1 = -x1;
      z  = x;
      z1 = x1;
      z += si;
      z1 += si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= si;
      z1 -= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= si;
      z1 *= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= si;
      z1 /= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      si = -si;
      z  = x;
      z1 = x1;
      z += si;
      z1 += si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= si;
      z1 -= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= si;
      z1 *= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= si;
      z1 /= si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      BOOST_CHECK_EQUAL((x == si), (x1 == si));
      BOOST_CHECK_EQUAL((x != si), (x1 != si));
      BOOST_CHECK_EQUAL((x <= si), (x1 <= si));
      BOOST_CHECK_EQUAL((x >= si), (x1 >= si));
      BOOST_CHECK_EQUAL((x < si), (x1 < si));
      BOOST_CHECK_EQUAL((x > si), (x1 > si));

      z = si;
      z1 = si;
      BOOST_CHECK_EQUAL((x == si), (x1 == si));
      BOOST_CHECK_EQUAL((x != si), (x1 != si));
      BOOST_CHECK_EQUAL((x <= si), (x1 <= si));
      BOOST_CHECK_EQUAL((x >= si), (x1 >= si));
      BOOST_CHECK_EQUAL((x < si), (x1 < si));
      BOOST_CHECK_EQUAL((x > si), (x1 > si));
   }

   void t3()
   {
      //
      // Mixed with unsigned integer:
      //
      boost::multiprecision::mpq_rational x(a, b), y(c, d), z;
      boost::multiprecision::cpp_rational x1(a1, b1), y1(c1, d1), z1;

      BOOST_CHECK_EQUAL(x.str(), x1.str());
      BOOST_CHECK_EQUAL(y.str(), y1.str());

      // Both positive:
      z  = x + ui;
      z1 = x1 + ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - ui;
      z1 = x1 - ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * ui;
      z1 = x1 * ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / ui;
      z1 = x1 / ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui + x;
      z1 = ui + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui - x;
      z1 = ui - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui * x;
      z1 = ui * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui / x;
      z1 = ui / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      x  = -x;
      x1 = -x1;
      z  = x + ui;
      z1 = x1 + ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - ui;
      z1 = x1 - ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * ui;
      z1 = x1 * ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / ui;
      z1 = x1 / ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui + x;
      z1 = ui + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui - x;
      z1 = ui - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui * x;
      z1 = ui * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = ui / x;
      z1 = ui / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      x = -x;
      x1 = -x1;
      // Inplace:
      z  = x;
      z1 = x1;
      z += ui;
      z1 += ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= ui;
      z1 -= ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= ui;
      z1 *= ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= ui;
      z1 /= ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      x  = -x;
      x1 = -x1;
      z  = x;
      z1 = x1;
      z += ui;
      z1 += ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= ui;
      z1 -= ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= ui;
      z1 *= ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= ui;
      z1 /= ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      x = -x;
      x1 = -x1;

      BOOST_CHECK_EQUAL((x == ui), (x1 == ui));
      BOOST_CHECK_EQUAL((x != ui), (x1 != ui));
      BOOST_CHECK_EQUAL((x <= ui), (x1 <= ui));
      BOOST_CHECK_EQUAL((x >= ui), (x1 >= ui));
      BOOST_CHECK_EQUAL((x < ui), (x1 < ui));
      BOOST_CHECK_EQUAL((x > ui), (x1 > ui));

      z  = ui;
      z1 = ui;
      BOOST_CHECK_EQUAL((x == ui), (x1 == ui));
      BOOST_CHECK_EQUAL((x != ui), (x1 != ui));
      BOOST_CHECK_EQUAL((x <= ui), (x1 <= ui));
      BOOST_CHECK_EQUAL((x >= ui), (x1 >= ui));
      BOOST_CHECK_EQUAL((x < ui), (x1 < ui));
      BOOST_CHECK_EQUAL((x > ui), (x1 > ui));
   }

   void t4()
   {
      //
      // Mixed with unsigned long integer:
      //
      boost::multiprecision::mpq_rational x(a, b), y(c, d), z;
      boost::multiprecision::cpp_rational x1(a1, b1), y1(c1, d1), z1;

      BOOST_CHECK_EQUAL(x.str(), x1.str());
      BOOST_CHECK_EQUAL(y.str(), y1.str());

      // Both positive:
      z  = x + large_ui;
      z1 = x1 + large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - large_ui;
      z1 = x1 - large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * large_ui;
      z1 = x1 * large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / large_ui;
      z1 = x1 / large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui + x;
      z1 = large_ui + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui - x;
      z1 = large_ui - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui * x;
      z1 = large_ui * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui / x;
      z1 = large_ui / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      x  = -x;
      x1 = -x1;
      z  = x + large_ui;
      z1 = x1 + large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - large_ui;
      z1 = x1 - large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * large_ui;
      z1 = x1 * large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / large_ui;
      z1 = x1 / large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui + x;
      z1 = large_ui + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui - x;
      z1 = large_ui - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui * x;
      z1 = large_ui * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = large_ui / x;
      z1 = large_ui / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      x  = -x;
      x1 = -x1;
      // Inplace:
      z  = x;
      z1 = x1;
      z += large_ui;
      z1 += large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= large_ui;
      z1 -= large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= large_ui;
      z1 *= large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= large_ui;
      z1 /= large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      x  = -x;
      x1 = -x1;
      z  = x;
      z1 = x1;
      z += large_ui;
      z1 += large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= large_ui;
      z1 -= large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= large_ui;
      z1 *= large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= large_ui;
      z1 /= large_ui;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      x  = -x;
      x1 = -x1;

      BOOST_CHECK_EQUAL((x == large_ui), (x1 == large_ui));
      BOOST_CHECK_EQUAL((x != large_ui), (x1 != large_ui));
      BOOST_CHECK_EQUAL((x <= large_ui), (x1 <= large_ui));
      BOOST_CHECK_EQUAL((x >= large_ui), (x1 >= large_ui));
      BOOST_CHECK_EQUAL((x < large_ui), (x1 < large_ui));
      BOOST_CHECK_EQUAL((x > large_ui), (x1 > large_ui));

      z  = large_ui;
      z1 = large_ui;
      BOOST_CHECK_EQUAL((x == large_ui), (x1 == large_ui));
      BOOST_CHECK_EQUAL((x != large_ui), (x1 != large_ui));
      BOOST_CHECK_EQUAL((x <= large_ui), (x1 <= large_ui));
      BOOST_CHECK_EQUAL((x >= large_ui), (x1 >= large_ui));
      BOOST_CHECK_EQUAL((x < large_ui), (x1 < large_ui));
      BOOST_CHECK_EQUAL((x > large_ui), (x1 > large_ui));
   }

   void t5()
   {
      //
      // Special cases:
      //
      boost::multiprecision::mpq_rational x(a, b), y(c, d), z;
      boost::multiprecision::cpp_rational x1(a1, b1), y1(c1, d1), z1;

      BOOST_CHECK_EQUAL(x.str(), x1.str());
      BOOST_CHECK_EQUAL(y.str(), y1.str());

      BOOST_CHECK_EQUAL(x1 * 0, 0);
      BOOST_CHECK_EQUAL(x1 * 1, x1);
      BOOST_CHECK_EQUAL(x1 * -1, -x1);

      z = x * y;
      z1 = x1;
      x1 = x1 * y1;
      BOOST_CHECK_EQUAL(z.str(), x1.str());
      x1 = z1;

      x1 = y1 * x1;
      BOOST_CHECK_EQUAL(z.str(), x1.str());
      x1 = z1;

      z = x * si;
      x1 = x1 * si;
      BOOST_CHECK_EQUAL(z.str(), x1.str());
      x1 = z1;

      x1 = si * x1;
      BOOST_CHECK_EQUAL(z.str(), x1.str());
      x1 = z1;

      z1 = x1;
      z1 *= 0;
      BOOST_CHECK_EQUAL(z1, 0);
      z1 = x1;
      z1 *= 1;
      BOOST_CHECK_EQUAL(z1, x1);
      z1 = x1;
      z1 *= -1;
      BOOST_CHECK_EQUAL(z1, -x1);

      z1 = x1 / x1;
      BOOST_CHECK_EQUAL(z1, 1);
      z1 = x1 / -x1;
      BOOST_CHECK_EQUAL(z1, -1);
      z = x / y;
      z1 = x1;
      z1 = z1 / y1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z1 = y1;
      z1 = x1 / z1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      z = x / si;
      z1 = x1;
      z1 = z1 / si;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z = si / x;
      z1 = x1;
      z1 = si / z1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      BOOST_CHECK_THROW(z1 = x1 / 0, std::overflow_error);
      z1= x1;
      BOOST_CHECK_THROW(z1 /= 0, std::overflow_error);
      z1 = x1;
      z1 /= 1;
      BOOST_CHECK_EQUAL(z1, x1);
      z1 /= -1;
      BOOST_CHECK_EQUAL(z1, -x1);
      z1 = x1 / 1;
      BOOST_CHECK_EQUAL(z1, x1);
      z1 = x1 / -1;
      BOOST_CHECK_EQUAL(z1, -x1);

      z1 = 0;
      BOOST_CHECK_EQUAL(z1 * si, 0);
      BOOST_CHECK_EQUAL(z1 / si, 0);
      BOOST_CHECK_EQUAL(z1 + si, si);
      BOOST_CHECK_EQUAL(z1 - si, -si);
      z1 *= si;
      BOOST_CHECK_EQUAL(z1, 0);
      z1 /= si;
      BOOST_CHECK_EQUAL(z1, 0);
      z1 += si;
      BOOST_CHECK_EQUAL(z1, si);
      z1 = 0;
      z1 -= si;
      BOOST_CHECK_EQUAL(z1, -si);

      z1 = si;
      z1 /= si;
      BOOST_CHECK_EQUAL(z1, 1);
      z1 = si;
      z1 /= -si;
      BOOST_CHECK_EQUAL(z1, -1);
      z1 = -si;
      z1 /= si;
      BOOST_CHECK_EQUAL(z1, -1);
      z1 = -si;
      z1 /= -si;
      BOOST_CHECK_EQUAL(z1, 1);

      x1 = si;
      z1 = si / x1;
      BOOST_CHECK_EQUAL(z1, 1);
      x1 = si;
      z1 = x1 / si;
      BOOST_CHECK_EQUAL(z1, 1);
      x1 = -si;
      z1 = si / x1;
      BOOST_CHECK_EQUAL(z1, -1);
      x1 = -si;
      z1 = x1 / si;
      BOOST_CHECK_EQUAL(z1, -1);
      x1 = -si;
      z1 = -si / x1;
      BOOST_CHECK_EQUAL(z1, 1);
      x1 = -si;
      z1 = x1 / -si;
      BOOST_CHECK_EQUAL(z1, 1);
      x1 = si;
      z1 = -si / x1;
      BOOST_CHECK_EQUAL(z1, -1);
      x1 = si;
      z1 = x1 / -si;
      BOOST_CHECK_EQUAL(z1, -1);
   }

   void t6()
   {
      //
      // Mixed with signed integer:
      //
      boost::multiprecision::mpq_rational x(a, b), y(c, d), z;
      boost::multiprecision::cpp_rational x1(a1, b1), y1(c1, d1), z1;

      boost::multiprecision::mpz_int bi = local_random::generate_random<boost::multiprecision::mpz_int>(1000);
      boost::multiprecision::cpp_int bi1(bi.str());

      BOOST_CHECK_EQUAL(x.str(), x1.str());
      BOOST_CHECK_EQUAL(y.str(), y1.str());
      BOOST_CHECK_EQUAL(bi.str(), bi1.str());

      // Both positive:
      z  = x + bi;
      z1 = x1 + bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - bi;
      z1 = x1 - bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * bi;
      z1 = x1 * bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / bi;
      z1 = x1 / bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi + x;
      z1 = bi1 + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi - x;
      z1 = bi1 - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi * x;
      z1 = bi1 * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi / x;
      z1 = bi1 / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      x  = -x;
      x1 = -x1;
      z  = x + bi;
      z1 = x1 + bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - bi;
      z1 = x1 - bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * bi;
      z1 = x1 * bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / bi;
      z1 = x1 / bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi + x;
      z1 = bi1 + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi - x;
      z1 = bi1 - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi * x;
      z1 = bi1 * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi / x;
      z1 = bi1 / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x and bi both negative:
      z  = x + bi;
      z1 = x1 + bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - bi;
      z1 = x1 - bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * bi;
      z1 = x1 * bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / bi;
      z1 = x1 / bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi + x;
      z1 = bi1 + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi - x;
      z1 = bi1 - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi * x;
      z1 = bi1 * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi / x;
      z1 = bi1 / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // bi negative:
      x  = -x;
      x1 = -x1;
      z  = x + bi;
      z1 = x1 + bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x - bi;
      z1 = x1 - bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x * bi;
      z1 = x1 * bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x / bi;
      z1 = x1 / bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi + x;
      z1 = bi1 + x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi - x;
      z1 = bi1 - x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi * x;
      z1 = bi1 * x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = bi / x;
      z1 = bi1 / x1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      bi = -bi;
      bi1 = -bi1;
      // Inplace:
      z  = x;
      z1 = x1;
      z += bi;
      z1 += bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= bi;
      z1 -= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= bi;
      z1 *= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= bi;
      z1 /= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // bi negative:
      bi = -bi;
      bi1 = -bi1;
      z  = x;
      z1 = x1;
      z += bi;
      z1 += bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= bi;
      z1 -= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= bi;
      z1 *= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= bi;
      z1 /= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // Both negative:
      x  = -x;
      x1 = -x1;
      z  = x;
      z1 = x1;
      z += bi;
      z1 += bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= bi;
      z1 -= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= bi;
      z1 *= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= bi;
      z1 /= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      // x negative:
      bi = -bi;
      bi1 = -bi1;
      z  = x;
      z1 = x1;
      z += bi;
      z1 += bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z -= bi;
      z1 -= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z *= bi;
      z1 *= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());
      z  = x;
      z1 = x1;
      z /= bi;
      z1 /= bi1;
      BOOST_CHECK_EQUAL(z.str(), z1.str());

      BOOST_CHECK_EQUAL((x == bi), (x1 == bi1));
      BOOST_CHECK_EQUAL((x != bi), (x1 != bi1));
      BOOST_CHECK_EQUAL((x <= bi), (x1 <= bi1));
      BOOST_CHECK_EQUAL((x >= bi), (x1 >= bi1));
      BOOST_CHECK_EQUAL((x < bi), (x1 < bi1));
      BOOST_CHECK_EQUAL((x > bi), (x1 > bi1));

      z  = bi;
      z1 = bi1;
      BOOST_CHECK_EQUAL((x == bi), (x1 == bi1));
      BOOST_CHECK_EQUAL((x != bi), (x1 != bi1));
      BOOST_CHECK_EQUAL((x <= bi), (x1 <= bi1));
      BOOST_CHECK_EQUAL((x >= bi), (x1 >= bi1));
      BOOST_CHECK_EQUAL((x < bi), (x1 < bi1));
      BOOST_CHECK_EQUAL((x > bi), (x1 > bi1));
   }

   void test()
   {
      using namespace boost::multiprecision;

      last_error_count = 0;

      BOOST_CHECK_EQUAL(Number(), 0);

      constexpr auto ilim = 10000;

      constexpr auto large_ui_digits_to_get = double_limb_type_digit_counter();

      // Ensure at compile-time that the amount of digits to get for large_ui is OK.
      static_assert(large_ui_digits_to_get >= std::numeric_limits<unsigned>::digits,
                    "Error: Ensure that the amount of digits to get for large_ui is really correct.");

      for (auto i = 0; i < ilim; ++i)
      {
         a = local_random::generate_random<mpz_int>(1000);
         b = local_random::generate_random<mpz_int>(1000);
         c = local_random::generate_random<mpz_int>(1000);
         d = local_random::generate_random<mpz_int>(1000);

         si = local_random::generate_random<int>
              (
                 static_cast<unsigned>(std::numeric_limits<int>::digits - 2)
              );

         ui = local_random::generate_random<unsigned>
              (
                 static_cast<unsigned>(std::numeric_limits<unsigned>::digits - 2)
              );

         large_ui =
              local_random::generate_random<boost::multiprecision::double_limb_type>
              (
                 static_cast<unsigned>(large_ui_digits_to_get - 2)
              );

         const auto a_represented_as_string = a.str();
         const auto b_represented_as_string = b.str();
         const auto c_represented_as_string = c.str();
         const auto d_represented_as_string = d.str();

         a1 = static_cast<test_type>(a_represented_as_string);
         b1 = static_cast<test_type>(b_represented_as_string);
         c1 = static_cast<test_type>(c_represented_as_string);
         d1 = static_cast<test_type>(d_represented_as_string);

         #if defined(TEST1)
         t1();
         #endif

         #if defined(TEST2)
         t2();
         #endif

         #if defined(TEST3)
         t3();
         #endif

         #if defined(TEST4)
         t4();
         #endif

         #if defined(TEST5)
         t5();
         #endif

         #if defined(TEST6)
         t6();
         #endif

         if (last_error_count != static_cast<unsigned>(boost::detail::test_errors()))
         {
            last_error_count = boost::detail::test_errors();
            std::cout << std::hex << std::showbase;

            std::cout << "a    = " << a << std::endl;
            std::cout << "a1   = " << a1 << std::endl;
            std::cout << "b    = " << b << std::endl;
            std::cout << "b1   = " << b1 << std::endl;
            std::cout << "c    = " << c << std::endl;
            std::cout << "c1   = " << c1 << std::endl;
            std::cout << "d    = " << d << std::endl;
            std::cout << "d1   = " << d1 << std::endl;
            std::cout << "a + b   = " << a + b << std::endl;
            std::cout << "a1 + b1 = " << a1 + b1 << std::endl;
            std::cout << std::dec;
            std::cout << "a - b   = " << a - b << std::endl;
            std::cout << "a1 - b1 = " << a1 - b1 << std::endl;
            std::cout << "-a + b   = " << mpz_int(-a) + b << std::endl;
            std::cout << "-a1 + b1 = " << test_type(-a1) + b1 << std::endl;
            std::cout << "-a - b   = " << mpz_int(-a) - b << std::endl;
            std::cout << "-a1 - b1 = " << test_type(-a1) - b1 << std::endl;
            std::cout << "c*d    = " << c * d << std::endl;
            std::cout << "c1*d1  = " << c1 * d1 << std::endl;
            std::cout << "b*c    = " << b * c << std::endl;
            std::cout << "b1*c1  = " << b1 * c1 << std::endl;
            std::cout << "a/b    = " << a / b << std::endl;
            std::cout << "a1/b1  = " << a1 / b1 << std::endl;
            std::cout << "a/d    = " << a / d << std::endl;
            std::cout << "a1/d1  = " << a1 / d1 << std::endl;
            std::cout << "a%b    = " << a % b << std::endl;
            std::cout << "a1%b1  = " << a1 % b1 << std::endl;
            std::cout << "a%d    = " << a % d << std::endl;
            std::cout << "a1%d1  = " << a1 % d1 << std::endl;
         }

         //
         // Check to see if test is taking too long.
         // Tests run on the compiler farm time out after 300 seconds,
         // so don't get too close to that:
         //
#ifndef CI_SUPPRESS_KNOWN_ISSUES
         if (tim.elapsed() > timer_type::seconds(180))
#else
         if (tim.elapsed() > timer_type::seconds(20))
#endif
         {
            std::cout << "Timeout reached, aborting tests now....\n";
            std::cout << "Loop count reached is i: " << i << "\n";
            break;
         }
      }
   }
};

int main()
{
   using namespace boost::multiprecision;

   tester<cpp_int> t1;
   t1.test();
   return boost::report_errors();
}
