//  (C) Copyright Beman Dawes 2003.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

int main()
{
  std::cout << "example of output before a <note> line\n";
  std::cout << "<note>\n";
  std::cout << "example of output after a <note> line\n";
  return 1;
}
