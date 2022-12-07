
// Copyright 2005-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_UNORDERED_TEST_HELPERS_METAFUNCTIONS_HEADER)
#define BOOST_UNORDERED_TEST_HELPERS_METAFUNCTIONS_HEADER

#include <boost/config.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/declval.hpp>

namespace test {
  template <class Container>
  struct is_set : public boost::is_same<typename Container::key_type,
                    typename Container::value_type>
  {
  };

  template <class Container> struct has_unique_keys
  {
    static char flip(typename Container::iterator const&);
    static long flip(std::pair<typename Container::iterator, bool> const&);
    BOOST_STATIC_CONSTANT(bool,
      value = sizeof(long) ==
              sizeof(flip(
                (boost::declval<Container*>())
                  ->insert(
                    boost::declval<typename Container::value_type const&>()))));
  };
}

#endif
