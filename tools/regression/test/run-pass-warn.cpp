//  (C) Copyright Beman Dawes 2003.
//  See accompanying license for terms and conditions of use.

int main()
{
  // provoke a compiler warning
  char x = 0xffffffff;
  return x ? 0 : 0; // use x to eliminate other warnings
}
