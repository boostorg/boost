//  (C) Copyright Beman Dawes 2003.
//  See accompanying license for terms and conditions of use.

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
