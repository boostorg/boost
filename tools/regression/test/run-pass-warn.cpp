//  (C) Copyright Beman Dawes 2003.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

int main()
{
  // provoke a compiler warning
  char x = 0xffffffff;
  return x ? 0 : 0; // use x to eliminate other warnings
}
