/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/default_allocator.hpp>
#include <boost/core/allocator_access.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    boost::default_allocator<int> a;
    int* p = boost::allocator_allocate(a, 1, 0);
    BOOST_TEST(p != 0);
    a.deallocate(p, 1);
    return boost::report_errors();
}
