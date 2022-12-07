/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/config.hpp>
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#include <boost/functional/identity.hpp>
#include <boost/core/lightweight_test.hpp>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

enum kind {
    rvalue,
    crvalue,
    other
};

kind result(std::string&&)
{
    return rvalue;
}

kind result(const std::string&&)
{
    return crvalue;
}

template<class T>
kind result(T&&)
{
    return other;
}

void simple_test()
{
    typedef std::string string;
    BOOST_TEST(boost::identity()(string("a")) == string("a"));
    BOOST_TEST(result(boost::identity()(string("a"))) == rvalue);
    typedef const std::string cstring;
    BOOST_TEST(boost::identity()(cstring("a")) == cstring("a"));
    BOOST_TEST(result(boost::identity()(cstring("a"))) == crvalue);
}

void algorithm_test()
{
    std::vector<std::string> v1;
    v1.push_back(std::string("a"));
    v1.push_back(std::string("b"));
    v1.push_back(std::string("c"));
    std::vector<std::string> v2(v1);
    std::vector<std::string> v3;
    std::transform(std::make_move_iterator(v2.begin()),
        std::make_move_iterator(v2.end()),
        std::back_inserter(v3),
        boost::identity());
    BOOST_TEST(v3 == v1);
}

int main()
{
    simple_test();
    algorithm_test();
    return boost::report_errors();
}
#else
int main()
{
    return 0;
}
#endif
