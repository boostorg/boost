//  (C) Copyright Beman Dawes 2003. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

#include <iostream>

int main()
{
  std::cout << "example of output before a <note> line\n";
  std::cout << "<note>\n";
  std::cout << "example of output after a <note> line\n";

  // provoke a compiler warning to make sure <note> takes priority over
  // a warning, but neither is lost from status reporting links HTML.
  char x = 0xffffffff;
  return x ? 0 : 0; // use x to eliminate other warnings
}
