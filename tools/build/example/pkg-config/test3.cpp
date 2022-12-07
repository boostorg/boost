// Copyright 2019 Dmitry Arkhipov
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)


#include <string>
#include <iostream>

int main(int, char const** argv) {
  return VARIANT == std::string(argv[1]) ? EXIT_SUCCESS : EXIT_FAILURE;
}
