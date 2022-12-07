// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/lambda/bind.hpp>
#include <boost/config.hpp>
#include <functional>

#if defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)

#include <boost/config/pragma_message.hpp>
BOOST_PRAGMA_MESSAGE( "Skipping test because BOOST_NO_CXX11_HDR_FUNCTIONAL is defined" )
int main() {}

#else

void func()
{
}

int main()
{
    std::function<void()>( boost::lambda::bind( func ) );
}

#endif
