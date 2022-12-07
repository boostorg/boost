// Copyright 2022 (c) Andrey Semashev
// Distributed under the Boost Software License Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/iterator/function_output_iterator.hpp>

namespace {

struct sum_func
{
    typedef void result_type;

    explicit sum_func(int& n) : m_n(n) {}
    result_type operator() (int x) const
    {
        m_n += x;
    }

private:
    int& m_n;
};

} // namespace

int main()
{
    {
        int n = 0;
        boost::iterators::function_output_iterator< sum_func > it =
            boost::iterators::make_function_output_iterator(sum_func(n));

        *it = 1;
        ++it;
        *it = 2;
        ++it;
        *it = 3;

        BOOST_TEST_EQ(n, 6);
    }

#if !defined(BOOST_NO_CXX11_LAMBDAS) && !defined(BOOST_NO_CXX11_AUTO_DECLARATIONS)
    {
        int n = 0;
        auto it = boost::iterators::make_function_output_iterator([&n](int x) { n -= x; });

        *it = 1;
        ++it;
        *it = 2;
        ++it;
        *it = 3;

        BOOST_TEST_EQ(n, -6);
    }
#endif

    return boost::report_errors();
}
