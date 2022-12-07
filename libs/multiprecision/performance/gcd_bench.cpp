//  Copyright 2020 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/random.hpp>
#include <cmath>

#include <immintrin.h>

using namespace boost::multiprecision;
using namespace boost::random;

namespace boost {
   namespace multiprecision {
      namespace backends {

      template <unsigned MinBits1, unsigned MaxBits1, cpp_integer_type SignType1, cpp_int_check_type Checked1, class Allocator1>
      inline BOOST_MP_CXX14_CONSTEXPR typename std::enable_if<!is_trivial_cpp_int<cpp_int_backend<MinBits1, MaxBits1, SignType1, Checked1, Allocator1> >::value>::type
      eval_gcd_old(
          cpp_int_backend<MinBits1, MaxBits1, SignType1, Checked1, Allocator1>&       result,
          const cpp_int_backend<MinBits1, MaxBits1, SignType1, Checked1, Allocator1>& a,
          const cpp_int_backend<MinBits1, MaxBits1, SignType1, Checked1, Allocator1>& b)
      {
         using default_ops::eval_get_sign;
         using default_ops::eval_is_zero;
         using default_ops::eval_lsb;

         if (a.size() == 1)
         {
            eval_gcd(result, b, *a.limbs());
            return;
         }
         if (b.size() == 1)
         {
            eval_gcd(result, a, *b.limbs());
            return;
         }

         cpp_int_backend<MinBits1, MaxBits1, SignType1, Checked1, Allocator1> u(a), v(b);

         int s = eval_get_sign(u);

         /* GCD(0,x) := x */
         if (s < 0)
         {
            u.negate();
         }
         else if (s == 0)
         {
            result = v;
            return;
         }
         s = eval_get_sign(v);
         if (s < 0)
         {
            v.negate();
         }
         else if (s == 0)
         {
            result = u;
            return;
         }

         /* Let shift := lg K, where K is the greatest power of 2
   dividing both u and v. */

         unsigned us    = eval_lsb(u);
         unsigned vs    = eval_lsb(v);
         int      shift = (std::min)(us, vs);
         eval_right_shift(u, us);
         eval_right_shift(v, vs);

         do
         {
            /* Now u and v are both odd, so diff(u, v) is even.
      Let u = min(u, v), v = diff(u, v)/2. */
            s = u.compare(v);
            if (s > 0)
               u.swap(v);
            if (s == 0)
               break;

            while (((u.size() + 2 < v.size()) && (v.size() * 100 / u.size() > 105)) || ((u.size() <= 2) && (v.size() > 4)))
            {
               //
               // Speical case: if u and v differ considerably in size, then a Euclid step
               // is more efficient as we reduce v by several limbs in one go.
               // Unfortunately it requires an expensive long division:
               //
               eval_modulus(v, v, u);
               u.swap(v);
            }
            if (v.size() <= 2)
            {
               //
               // Special case: if v has no more than 2 limbs
               // then we can reduce u and v to a pair of integers and perform
               // direct integer gcd:
               //
               if (v.size() == 1)
                  u = eval_gcd(*v.limbs(), *u.limbs());
               else
               {
                  double_limb_type i = v.limbs()[0] | (static_cast<double_limb_type>(v.limbs()[1]) << sizeof(limb_type) * CHAR_BIT);
                  double_limb_type j = (u.size() == 1) ? *u.limbs() : u.limbs()[0] | (static_cast<double_limb_type>(u.limbs()[1]) << sizeof(limb_type) * CHAR_BIT);
                  u                  = eval_gcd(i, j);
               }
               break;
            }
            //
            // Regular binary gcd case:
            //
            eval_subtract(v, u);
            vs = eval_lsb(v);
            eval_right_shift(v, vs);
         } while (true);

         result = u;
         eval_left_shift(result, shift);
      }

      }
   }
}

template <class T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<T> >& get_test_vector(unsigned bits)
{
   static std::map<unsigned, std::tuple<std::vector<T>, std::vector<T>, std::vector<T> > > data;

   std::tuple<std::vector<T>, std::vector<T>, std::vector<T> >& result = data[bits];

   if (std::get<0>(result).size() == 0)
   {
      mt19937                     mt;
      uniform_int_distribution<T> ui(T(1) << (bits - 1), T(1) << bits);

      std::vector<T>& a = std::get<0>(result);
      std::vector<T>& b = std::get<1>(result);
      std::vector<T>& c = std::get<2>(result);

      for (unsigned i = 0; i < 1000; ++i)
      {
         a.push_back(ui(mt));
         b.push_back(ui(mt));
         if (b.back() > a.back())
            b.back().swap(a.back());
         c.push_back(0);
      }
   }
   return result;
}

template <class T>
std::vector<T>& get_test_vector_a(unsigned bits)
{
   return std::get<0>(get_test_vector<T>(bits));
}
template <class T>
std::vector<T>& get_test_vector_b(unsigned bits)
{
   return std::get<1>(get_test_vector<T>(bits));
}
template <class T>
std::vector<T>& get_test_vector_c(unsigned bits)
{
   return std::get<2>(get_test_vector<T>(bits));
}


template <typename T>
static void BM_gcd_old(benchmark::State& state)
{
   int                         bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);
   std::vector<T>& c = get_test_vector_c<T>(bits);

   for (auto _ : state)
   {
      for (unsigned i = 0; i < a.size(); ++i)
         eval_gcd_old(c[i].backend(), a[i].backend(), b[i].backend());
   }
   state.SetComplexityN(bits);
}

template <typename T>
static void BM_gcd_current(benchmark::State& state)
{
   int                         bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);
   std::vector<T>& c = get_test_vector_c<T>(bits);

   for (auto _ : state)
   {
      for (unsigned i = 0; i < a.size(); ++i)
         eval_gcd(c[i].backend(), a[i].backend(), b[i].backend());
   }
   state.SetComplexityN(bits);
}

constexpr unsigned lower_range = 512;
constexpr unsigned upper_range = 1 << 15;

BENCHMARK_TEMPLATE(BM_gcd_old, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_gcd_current, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_gcd_old, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_gcd_current, mpz_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_gcd_current, mpz_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();

BENCHMARK_MAIN();
