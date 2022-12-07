/*
Copyright 2022 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/functional/identity.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/utility/result_of.hpp>

int main()
{
    BOOST_TEST_TRAIT_SAME(boost::result_of<boost::identity(int&)>::type, int&);
    BOOST_TEST_TRAIT_SAME(boost::result_of<boost::identity(const int&)>::type,
        const int&);
    return boost::report_errors();
}
