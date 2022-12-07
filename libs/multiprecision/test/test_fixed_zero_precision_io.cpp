// Copyright John Maddock 2022.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif
#define BOOST_CHRONO_HEADER_ONLY

#ifdef TEST_BIN_FLOAT
#include <boost/multiprecision/cpp_bin_float.hpp>
#endif
#ifdef TEST_DEC_FLOAT
#include <boost/multiprecision/cpp_dec_float.hpp>
#endif
#ifdef TEST_MPFR
#include <boost/multiprecision/mpfr.hpp>
#endif
#ifdef TEST_MPF
#include <boost/multiprecision/gmp.hpp>
#endif
#ifdef TEST_FLOAT128
#include <boost/multiprecision/float128.hpp>
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/chrono.hpp>
#include "test.hpp"
#include <boost/array.hpp>
#include <iostream>
#include <iomanip>

#ifdef BOOST_MSVC
#pragma warning(disable : 4127)
#endif

#if defined(TEST_MPF)
template <unsigned N, boost::multiprecision::expression_template_option ET>
bool has_bad_bankers_rounding(const boost::multiprecision::number<boost::multiprecision::gmp_float<N>, ET>&)
{
   return true;
}
#endif
template <class T>
bool has_bad_bankers_rounding(const T&)
{
   return false;
}

bool is_bankers_rounding_error(const std::string& s, const std::string& expect)
{
   // This check isn't foolproof: that would require *much* more sophisticated code!!!
   std::string::size_type l = expect.size();
   if (l != s.size())
      return false;
   std::string::size_type len = s.find('e');
   if (len == std::string::npos)
      len = l - 1;
   else
      --len;
   for (unsigned i = 0; i < len; ++i)
   {
      char c1 = s[i];
      char c2 = expect[i];
      if (c1 != c2)
         return false;
   }
   if (s[len] != expect[len] + 1)
      return false;
   return true;
}

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

template <class T>
struct exponent_type
{
   typedef int type;
};
template <class T, boost::multiprecision::expression_template_option ET>
struct exponent_type<boost::multiprecision::number<T, ET> >
{
   typedef typename T::exponent_type type;
};

template <class T>
T generate_random_float()
{
   BOOST_MATH_STD_USING
   typedef typename exponent_type<T>::type e_type;
   static boost::random::mt19937           gen;
   T                                       val      = gen();
   T                                       prev_val = -1;
   while (val != prev_val)
   {
      val *= (gen.max)();
      prev_val = val;
      val += gen();
   }
   e_type e;
   val = frexp(val, &e);

   static const int                                       max_exponent_value = (std::min)(static_cast<int>(std::numeric_limits<T>::max_exponent - std::numeric_limits<T>::digits - 20), 40);
   static boost::random::uniform_int_distribution<e_type> ui(0, max_exponent_value);
   return ldexp(val, ui(gen));
}

template <class Float>
void test_fixed_io()
{
   std::cout << "Testing type " << typeid(Float).name() << std::endl;
   std::cout << "digits = " << std::numeric_limits<Float>::digits << std::endl;
   std::cout << "digits10 = " << std::numeric_limits<Float>::digits10 << std::endl;
   std::cout << "max_digits10 = " << std::numeric_limits<Float>::max_digits10 << std::endl;

   stopwatch<boost::chrono::high_resolution_clock> w;

   int count = 0;

#ifndef CI_SUPPRESS_KNOWN_ISSUES
   while (boost::chrono::duration_cast<boost::chrono::duration<double> >(w.elapsed()).count() < 200)
#else
   while (boost::chrono::duration_cast<boost::chrono::duration<double> >(w.elapsed()).count() < 50)
#endif
   {
      double val = generate_random_float<double>();
      Float  f(val);
#ifdef BOOST_MSVC
      if (val > 0.5 && val < 1)
         val = 1;
#endif
      std::stringstream ss1, ss2;
      ss1 << std::fixed << std::setprecision(0) << val;
      ss2 << std::fixed << std::setprecision(0) << f;
      if (ss1.str() != ss2.str())
      {
         if (has_bad_bankers_rounding(Float()) && is_bankers_rounding_error(ss2.str(), ss1.str()))
         {
            std::cout << "Ignoring bankers-rounding error with GMP mp_f.\n";
         }
         else
         {
            std::cout << std::setprecision(20) << "Testing value " << val << std::endl;
            std::cout << "Got:      " << ss2.str() << std::endl;
            std::cout << "Expected: " << ss1.str() << std::endl;
            ++boost::detail::test_errors();
            Float(val).str(0, std::ios_base::fixed);
         }
      }
      ++count;
      if (boost::detail::test_errors() > 100)
         break;
   }

   std::cout << "Execution time = " << boost::chrono::duration_cast<boost::chrono::duration<double> >(w.elapsed()).count() << "s" << std::endl;
   std::cout << "Total values tested: " << count << std::endl;
}


int main()
{
#ifdef BOOST_MSVC && (BOOST_MSVC < 1920)
   std::cout << "MSVC prior to 14.2 does not perform bankers rounding for double IO, as a result all our test cases are incorrect, so there's nothing we can productively test here." << std::endl;
#else
   using namespace boost::multiprecision;
#ifdef TEST_BIN_FLOAT
   test_fixed_io<number<cpp_bin_float<113, digit_base_2, void, std::int16_t> > >();
#endif
#ifdef TEST_DEC_FLOAT
   test_fixed_io<cpp_dec_float_50>();
#endif
#ifdef TEST_MPFR
   test_fixed_io<boost::multiprecision::mpfr_float_50>();
#endif
#ifdef TEST_MPF
   test_fixed_io<boost::multiprecision::mpf_float_50>();
#endif
#ifdef TEST_FLOAT128
   test_fixed_io<boost::multiprecision::float128>();
#endif
#endif
   return boost::report_errors();
}
