//  (C) Copyright Beman Dawes 2003.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

int main()
{
  std::cout << "example of output before a <note> line\n";
  std::cout << "<note>\n";
  std::cout << "example of output after a <note> line\n";

  // provoke a compiler warning to make sure <note> takes priority over
  // a warning, but neither is lost from status reporting links HTML.
  short s;
  unsigned long ul;
  s = s & ul; // warning from many compilers
  if ( s == ul ) {} // warning from GCC
  return 0;
}
