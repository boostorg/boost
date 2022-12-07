///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "sf_performance.hpp"

#ifdef __clang__
#if __has_feature(address_sanitizer) || __has_feature(memory_sanitizer)
// memory sanitizer is incompatible with this test:
# define DISABLE_THIS_TEST
#endif
#endif

boost::atomic<unsigned>                                                     allocation_count(0);
std::map<std::string, std::map<std::string, std::pair<double, unsigned> > > result_table;

#ifndef DISABLE_THIS_TEST

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

void* operator new(std::size_t count)
{
   ++allocation_count;
   return std::malloc(count);
}

void* operator new[](std::size_t count)
{
   ++allocation_count;
   return std::malloc(count);
}

void operator delete(void* ptr)noexcept
{
   std::free(ptr);
}
void operator delete[](void* ptr) noexcept
{
   std::free(ptr);
}

void print_quickbook_tables()
{
   for (auto i = result_table.begin(); i != result_table.end(); ++i)
   {
      std::cout << "[table " << i->first << "\n";
      std::cout << "[[Type][Time][# Allocations]]\n";
      double min_time = (std::numeric_limits<double>::max)();
      for (auto j = i->second.begin(); j != i->second.end(); ++j)
      {
         if (j->second.first < min_time)
            min_time = j->second.first;
      }
      for (auto j = i->second.begin(); j != i->second.end(); ++j)
      {
         double t = j->second.first;
         std::cout << "[[" << j->first << "][" << t / min_time << " (" << t << "s)][" << j->second.second << "]]\n";
      }
      std::cout << "]\n\n";
   }
}

int main()
{
   using namespace boost::multiprecision;

#if defined(TEST_MPFR) || defined(TEST_MPFR_CLASS) || defined(TEST_MPREAL) || defined(TEST_MPF)
   mp_get_memory_functions(&alloc_func_ptr, &realloc_func_ptr, &free_func_ptr);
   mp_set_memory_functions(&alloc_func, &realloc_func, &free_func);
#endif

   basic_tests_1();
   basic_tests_2();
   basic_tests_3();
   basic_tests_4();
   basic_tests_5();
   basic_tests_6();
   basic_tests_7();
   basic_tests_8();
   basic_tests_9();
   bessel_tests();
   poly_tests();
   nct_tests();

   print_quickbook_tables();
}

#else
int main() { return 0; }
#endif
