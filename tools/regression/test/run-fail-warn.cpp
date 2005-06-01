//  (C) Copyright Beman Dawes 2003.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

int main()
{
  short s;
  unsigned long ul;
  s = s & ul; // warning from many compilers
  if ( s == ul ) {} // warning from GCC

  std::cout << "example of output from a run-time failure\n";
  return 1;
}
