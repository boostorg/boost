//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <string>
#include <set>

#include <boost/locale.hpp>

using namespace boost::locale;

int main()
{
     generator gen;
     std::locale::global(gen(""));
     /// Set global locale to requested

     /// Create a set that includes all strings sorted according to ABC order
     /// std::locale can be used as object for comparison
     typedef std::set<std::string,std::locale> set_type;
     set_type all_strings;

     /// Read all strings into the set
     while(!std::cin.eof()) {
          std::string tmp;
          std::getline(std::cin,tmp);
          all_strings.insert(tmp);
     }
     /// Print them out
     for(set_type::iterator p=all_strings.begin();p!=all_strings.end();++p) {
          std::cout << *p << std::endl;
     }

}
