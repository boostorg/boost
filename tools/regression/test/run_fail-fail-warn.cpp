//  (C) Copyright Beman Dawes 2003.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

int main()
{
  short s;
  unsigned long ul;
  s = s & ul; // warning from many compilers
  if ( s == ul ) {} // warning from GCC
  return 0;
}
