// Copyright 2019 Dmitry Arkhipov
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)


#include <string>

int main() {
  return QWERTY == std::string("uiop") ? EXIT_SUCCESS : EXIT_FAILURE ;
}
