///////////////////////////////////////////////////////////////
//  Copyright 2012 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "miller_rabin_performance.hpp"

unsigned allocation_count = 0;

void* (*alloc_func_ptr)(size_t);
void* (*realloc_func_ptr)(void*, size_t, size_t);
void (*free_func_ptr)(void*, size_t);

void* alloc_func(size_t n)
{
   ++allocation_count;
   return (*alloc_func_ptr)(n);
}

void free_func(void* p, size_t n)
{
   (*free_func_ptr)(p, n);
}

void* realloc_func(void* p, size_t old, size_t n)
{
   ++allocation_count;
   return (*realloc_func_ptr)(p, old, n);
}

#ifdef TEST_MPZ
boost::chrono::duration<double> test_miller_rabin_gmp()
{
   using namespace boost::random;
   using namespace boost::multiprecision;

   stopwatch<boost::chrono::high_resolution_clock> c;

   independent_bits_engine<mt11213b, 256, mpz_int> gen;

   for (unsigned i = 0; i < 1000; ++i)
   {
      mpz_int n = gen();
      mpz_probab_prime_p(n.backend().data(), 25);
   }
   return c.elapsed();
}
#endif

std::map<std::string, double> results;
double                        min_time = (std::numeric_limits<double>::max)();

void generate_quickbook()
{
   std::cout << "[table\n[[Integer Type][Relative Performance (Actual time in parenthesis)]]\n";

   std::map<std::string, double>::const_iterator i(results.begin()), j(results.end());

   while (i != j)
   {
      double rel = i->second / min_time;
      std::cout << "[[" << i->first << "][" << rel << "(" << i->second << "s)]]\n";
      ++i;
   }

   std::cout << "]\n";
}

int main()
{
   test01();
   test02();
   test03();
   test04();
   test05();
   test06();
   test07();
   test08();
   test09();
   test10();
   test11();
   test12();

   generate_quickbook();

   return 0;
}
