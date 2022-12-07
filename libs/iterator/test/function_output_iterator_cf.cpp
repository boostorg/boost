// Copyright 2022 (c) Andrey Semashev
// Distributed under the Boost Software License Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
    int n = 0;
    boost::iterators::function_output_iterator< sum_func > it1 =
        boost::iterators::make_function_output_iterator(sum_func(n));
    boost::iterators::function_output_iterator< sum_func > it2 =
        boost::iterators::make_function_output_iterator(sum_func(n));

    *it1 = *it2;

    return 0;
}
