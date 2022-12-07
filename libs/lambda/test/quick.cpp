// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/lambda/lambda.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    using namespace boost::lambda;

    BOOST_TEST_EQ( (_1 + _2)(1, 2), 3 );

    return boost::report_errors();
}
