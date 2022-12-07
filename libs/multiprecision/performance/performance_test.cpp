///////////////////////////////////////////////////////////////
//  Copyright 2011-21 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

//
// This is the main entry point for our operator performance test suite.
// In order to build this program, you must compile and link this file against
// all the libs/multiprecision/performance/performance_test_files/*.cpp files.
// 
// The default behaviour is to "test everything", which is probably not what you want.
// In order to restict testing to a specific selection of backends, you will need to
// define one or more of the following macros when building:
// 
// TEST_MPF
// TEST_MPZ
// TEST_CPP_DEC_FLOAT
// TEST_MPFR
// TEST_MPQ
// TEST_TOMMATH
// TEST_TOMMATH_BOOST_RATIONAL
// TEST_MPZ_BOOST_RATIONAL
// TEST_CPP_INT
// TEST_CPP_INT_RATIONAL
// TEST_CPP_BIN_FLOAT
//

#include "performance_test.hpp"

#ifdef TEST_MPZ
#include <gmp.h>
#endif
#ifdef TEST_MPFR
#include <mpfr.h>
#endif
#include <boost/version.hpp>

//
// Keys in order are:
// Category
// Operator
// Type
// Precision
// Time
//
std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, double> > > > result_table;

unsigned bits_wanted; // for integer types

void quickbook_results()
{
   //
   // Keys in order are:
   // Category
   // Operator
   // Type
   // Precision
   // Time
   //
   typedef std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, double> > > >::const_iterator category_iterator;
   typedef std::map<std::string, std::map<std::string, std::map<int, double> > >::const_iterator                         operator_iterator;
   typedef std::map<std::string, std::map<int, double> >::const_iterator                                                 type_iterator;
   typedef std::map<int, double>::const_iterator                                                                         precision_iterator;

   for (category_iterator i = result_table.begin(); i != result_table.end(); ++i)
   {
      std::string cat = i->first;
      cat[0]          = (char)std::toupper((char)cat[0]);
      std::cout << "[section:" << i->first << "_performance " << cat << " Type Perfomance]" << std::endl;

      for (operator_iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
         std::string op = j->first;
         std::cout << "[table Operator " << op << std::endl;
         std::cout << "[[Backend]";

         for (precision_iterator k = j->second.begin()->second.begin(); k != j->second.begin()->second.end(); ++k)
         {
            std::cout << "[" << k->first << " Bits]";
         }
         std::cout << "]\n";

         std::vector<double> best_times(j->second.begin()->second.size(), (std::numeric_limits<double>::max)());
         for (unsigned m = 0; m < j->second.begin()->second.size(); ++m)
         {
            for (type_iterator k = j->second.begin(); k != j->second.end(); ++k)
            {
               if (m < k->second.size())
               {
                  precision_iterator l = k->second.begin();
                  std::advance(l, m);
                  if (best_times[m] > l->second)
                     best_times[m] = l->second ? l->second : best_times[m];
               }
            }
         }

         for (type_iterator k = j->second.begin(); k != j->second.end(); ++k)
         {
            std::cout << "[[" << k->first << "]";

            unsigned m = 0;
            for (precision_iterator l = k->second.begin(); l != k->second.end(); ++l)
            {
               double rel_time = l->second / best_times[m];
               if (rel_time == 1)
                  std::cout << "[[*" << rel_time << "]";
               else
                  std::cout << "[" << rel_time;
               std::cout << " (" << l->second << "s)]";
               ++m;
            }

            std::cout << "]\n";
         }

         std::cout << "]\n";
      }

      std::cout << "[endsect]" << std::endl;
   }
}

#if defined(__HAS_INCLUDE)
#if __has_include(<sys/utsname.h>)
#define HAS_UTSNAME
#include <sys/utsname.h>
#endif
#endif
#ifdef _WIN32
#include <windows.h>
#endif

void quickbook_platform_details()
{
   std::cout << "[table:platform Platform Details\n[[Platform][";
#ifdef HAS_UTSNAME
   utsname name;
   uname(&name);
   std::cout << name.sysname << " " << name.release << ", version " << name.version << ", " << name.machine << "]]\n";
#elif defined(_WIN32)
   std::cout << "Windows ";
#ifdef _M_AMD64
   std::cout << "x64";
#elif defined(_M_IX86)
   std::cout << "x86";
#endif
   std::cout << "]]\n";
#endif
   std::cout << "[[Compiler][" << BOOST_COMPILER << "]]\n";
#ifdef TEST_MPZ
   std::cout << "[[GMP][" << gmp_version << "]]\n";
#endif
#ifdef TEST_MPFR
   std::cout << "[[MPFR][" << MPFR_VERSION << "]]\n";
#endif
   std::cout << "[[Boost][" << BOOST_VERSION << "]]\n";
   std::cout << "[[Run date][" << __DATE__ << "]]\n";
   std::cout << "]\n\n";
}

int main()
{
   quickbook_platform_details();

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
   test13();
   test14();
   test15();
   test16();
   test17();
   test18();
   test19();
   test20();
   test21();
   test22();
   test23();
   test24();
   test25();
   test26();
   test27();
   test28();
   test29();
   test30();
   test31();
   test32();
   test33();
   test34();
   test35();
   test36();
   test37();
   test38();
   test39();
   test40();
   test41();
   test42();
   test43();
   test44();
   test45();
   test46();
   test47();
   test48();
   test49();
   test50();
   test51();

   quickbook_results();
   return 0;
}
