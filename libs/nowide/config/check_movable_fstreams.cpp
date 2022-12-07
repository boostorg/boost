//
// Copyright (c) 2020 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <fstream>
#include <utility>

/// Check that the stdlib supports swapping and moving fstreams
/// (and by extension all other streams and streambufs)
void check()
{
    std::fstream s1, s2;
    s1.swap(s2);
    s2 = std::move(s1);
}
