// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#if !defined(_WIN32) && !defined(__CYGWIN__)

#include <boost/config/pragma_message.hpp>
BOOST_PRAGMA_MESSAGE( "Skipping test because _WIN32 and __CYGWIN__ are not defined" )

#else

#include <windows.h>
#include <boost/core/detail/string_view.hpp>

#endif
