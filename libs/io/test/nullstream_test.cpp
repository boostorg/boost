/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/io/nullstream.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    {
        boost::io::onullstream s;
        s << "abc";
        s << 100;
        s << std::endl;
        BOOST_TEST(s);
    }
    {
        boost::io::wonullstream s;
        s << L"abc";
        s << 100;
        s << std::endl;
        BOOST_TEST(s);
    }
    return boost::report_errors();
}
