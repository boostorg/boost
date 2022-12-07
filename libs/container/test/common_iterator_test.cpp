//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2021-2022. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/config.hpp>

#if BOOST_CXX_VERSION >= 202002L

#include <version>

#if defined(__cpp_lib_concepts) //common_iterator

#define BOOST_CONTAINER_TEST_HAS_CONCEPTS

#endif
#endif

#if defined(BOOST_CONTAINER_TEST_HAS_CONCEPTS) //common_iterator

#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>
#include <boost/container/slist.hpp>
#include <boost/container/set.hpp>
#include <boost/container/stable_vector.hpp>
#include <vector>
#include <iterator>
#include <boost/core/lightweight_test.hpp>

int main()
{
    using Ints = std::vector<int>;
    Ints ints = { 1,2,3 };

    using I = std::counted_iterator<Ints::iterator>;
    using S = std::default_sentinel_t;
    using C = std::common_iterator<I, S>;
    using D = std::ptrdiff_t;
    boost::container::vector<int> c1(C(std::counted_iterator(ints.begin(), D(D(ints.size())))), C(std::default_sentinel));
    BOOST_TEST(c1.size() == ints.size());
    boost::container::vector c2(C(std::counted_iterator(ints.begin(), D(ints.size()))), C(std::default_sentinel));
    BOOST_TEST(c2.size() == ints.size());
    boost::container::list c3 (C(std::counted_iterator(ints.begin(), D(ints.size()))), C(std::default_sentinel));
    BOOST_TEST(c3.size() == ints.size());
    boost::container::slist c4 (C(std::counted_iterator(ints.begin(), D(ints.size()))), C(std::default_sentinel));
    BOOST_TEST(c4.size() == ints.size());
    boost::container::set c5(C(std::counted_iterator(ints.begin(), D(ints.size()))), C(std::default_sentinel));
    BOOST_TEST(c5.size() == ints.size());
    boost::container::stable_vector c6(C(std::counted_iterator(ints.begin(), D(ints.size()))), C(std::default_sentinel));
    BOOST_TEST(c6.size() == ints.size());

    return ::boost::report_errors();
}

#else

#include <boost/container/vector.hpp>

int main()
{
    return 0;
}

#endif
