/*  Copyright 2020 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

/*
This program is a compile test for support of C++11. If it compiles
successfully some key parts of C++11 the B2 engine requires are
available. This is used by the build script to guess and check the
compiler to build the engine with.
*/

// Some headers we test...
#include <thread>
#include <memory>


int main()
{
    // Check for basic thread calls.
    // [2020-08-19] Mingw-w64 with win32 threading model (as opposed to posix
    // threading model) does not really have std::thread etc. Please see comments
    // in sysinfo.cpp.
    #ifndef _WIN32
    { auto _ = std::thread::hardware_concurrency(); }
    #endif

    // [2021-08-07] We check the following C++11 features: brace initialization,
    // unique_ptr. Plus the author's ability to memorize some digits.
    { const std::unique_ptr <float> pf {new float {3.14159f}}; }
}
