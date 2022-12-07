/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/functional/identity.hpp>
#include <boost/core/lightweight_test.hpp>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

enum kind {
    ref,
    cref,
    other
};

kind result(std::string&)
{
    return ref;
}

kind result(const std::string&)
{
    return cref;
}

template<class T>
kind result(T&)
{
    return other;
}

template<class T>
kind result(const T&)
{
    return other;
}

void simple_test()
{
    std::string s1("a");
    BOOST_TEST(boost::identity()(s1) == s1);
    BOOST_TEST(&boost::identity()(s1) == &s1);
    BOOST_TEST(result(boost::identity()(s1)) == ref);
    const std::string s2("a");
    BOOST_TEST(boost::identity()(s2) == s2);
    BOOST_TEST(&boost::identity()(s2) == &s2);
    BOOST_TEST(result(boost::identity()(s2)) == cref);
}

void algorithm_test()
{
    std::vector<std::string> v1;
    v1.push_back(std::string("a"));
    v1.push_back(std::string("b"));
    v1.push_back(std::string("c"));
    std::vector<std::string> v2;
    std::transform(v1.begin(), v1.end(), std::back_inserter(v2),
         boost::identity());
    BOOST_TEST(v2 == v1);
}

int main()
{
    simple_test();
    algorithm_test();
    return boost::report_errors();
}
