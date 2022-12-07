// Copyright 2022 Dmitry Arkhipov (grisumbras@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt


#include <boost/mp11/utility.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <cstddef>

template<class T>
using has_size_t_difference_type
    = std::is_same<typename T::difference_type, std::size_t>;

struct X
{
};

struct Y
{
    using difference_type = int;
};

struct Z
{
    using difference_type = std::size_t;
};

int main()
{
    using boost::mp11::mp_valid_and_true;
    using boost::mp11::mp_valid_and_true_q;
    using boost::mp11::mp_quote;

    BOOST_TEST_TRAIT_FALSE((mp_valid_and_true<has_size_t_difference_type, X>));
    BOOST_TEST_TRAIT_FALSE((mp_valid_and_true<has_size_t_difference_type, Y>));
    BOOST_TEST_TRAIT_TRUE((mp_valid_and_true<has_size_t_difference_type, Z>));

    using Q_size_t_diff = mp_quote<has_size_t_difference_type>;

    BOOST_TEST_TRAIT_FALSE((mp_valid_and_true_q<Q_size_t_diff, X>));
    BOOST_TEST_TRAIT_FALSE((mp_valid_and_true_q<Q_size_t_diff, Y>));
    BOOST_TEST_TRAIT_TRUE((mp_valid_and_true_q<Q_size_t_diff, Z>));

    return boost::report_errors();
}
