//  (C) Copyright Beman Dawes 2003. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

int main()
{
  // provoke a compiler warning
  char x = 0xffffffff;
  return x ? 0 : 0; // use x to eliminate other warnings
}
