// Copyright (c) 2021 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

class Foo
{};

Foo foo __attribute__((init_priority(101)));

int main()
{
    return 0;
}
