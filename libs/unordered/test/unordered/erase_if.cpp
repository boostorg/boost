// Copyright 2021-2022 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// clang-format off
#include "../helpers/prefix.hpp"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../helpers/postfix.hpp"
// clang-format on

#include "../helpers/test.hpp"

#include <boost/config.hpp>
#include <string>

#if !defined(BOOST_NO_CXX11_REF_QUALIFIERS)
#define UNORDERED_LVALUE_QUAL &
#else
#define UNORDERED_LVALUE_QUAL
#endif

namespace test {
  struct is_even
  {
    is_even() {}

#if BOOST_CXX_VERSION >= 201703L
    // immovable for C++17
    is_even(is_even const&) = delete;
    is_even(is_even&&) = delete;

    is_even& operator=(is_even const&) = delete;
    is_even& operator=(is_even&&) = delete;
#elif BOOST_CXX_VERSION >= 201103L
    // move-only for C++11
    is_even(is_even const&) = delete;
    is_even(is_even&&) = default;

    is_even& operator=(is_even const&) = delete;
    is_even& operator=(is_even&&) = default;
#else
    // copyable otherwise
    is_even(is_even const&) {}
    is_even& operator=(is_even const&) { return *this; }
#endif

    bool operator()(
      std::pair<std::string const, int>& key_value) UNORDERED_LVALUE_QUAL
    {
      int const v = key_value.second;
      return (v % 2 == 0);
    }

    bool operator()(int const& value) UNORDERED_LVALUE_QUAL
    {
      int const v = value;
      return (v % 2 == 0);
    }
  };

  struct is_too_large
  {
    is_too_large() {}

#if BOOST_CXX_VERSION >= 201703L
    // immovable for C++17
    is_too_large(is_too_large const&) = delete;
    is_too_large(is_too_large&&) = delete;

    is_too_large& operator=(is_too_large const&) = delete;
    is_too_large& operator=(is_too_large&&) = delete;
#elif BOOST_CXX_VERSION >= 201103L
    // move-only for C++11
    is_too_large(is_too_large const&) = delete;
    is_too_large(is_too_large&&) = default;

    is_too_large& operator=(is_too_large const&) = delete;
    is_too_large& operator=(is_too_large&&) = default;
#else
    // copyable otherwise
    is_too_large(is_too_large const&) {}

    is_too_large& operator=(is_too_large const&) { return *this; }
#endif

    bool operator()(
      std::pair<std::string const, int>& key_value) UNORDERED_LVALUE_QUAL
    {
      int const v = key_value.second;
      return v >= 1000;
    }

    bool operator()(int const& value) UNORDERED_LVALUE_QUAL
    {
      int const v = value;
      return v >= 1000;
    }
  };

} // namespace test

template <class UnorderedMap> void test_map_erase_if()
{
  typedef UnorderedMap map_type;
  typedef typename map_type::size_type size_type;

  map_type map;
  size_type num_erased = erase_if(map, test::is_even());
  BOOST_TEST(map.empty());
  BOOST_TEST_EQ(num_erased, 0u);

  map.emplace("a", 1);
  map.emplace("b", 2);
  map.emplace("b", 4);
  map.emplace("b", 8);
  map.emplace("b", 16);
  map.emplace("c", 3);

  size_type size = map.size();

  num_erased = erase_if(map, test::is_too_large());

  BOOST_TEST_EQ(map.size(), size);
  BOOST_TEST_EQ(num_erased, 0u);

  num_erased = erase_if(map, test::is_even());
  BOOST_TEST_EQ(map.size(), 2u);
  BOOST_TEST_EQ(num_erased, size - map.size());
}

template <class UnorderedSet> void test_set_erase_if()
{
  typedef UnorderedSet set_type;
  typedef typename set_type::size_type size_type;

  set_type set;
  size_type num_erased = erase_if(set, test::is_even());
  BOOST_TEST(set.empty());
  BOOST_TEST_EQ(num_erased, 0u);

  set.emplace(1);
  set.emplace(2);
  set.emplace(2);
  set.emplace(2);
  set.emplace(2);
  set.emplace(3);

  size_type size = set.size();

  num_erased = erase_if(set, test::is_too_large());

  BOOST_TEST_EQ(set.size(), size);
  BOOST_TEST_EQ(num_erased, 0u);

  num_erased = erase_if(set, test::is_even());
  BOOST_TEST_EQ(set.size(), 2u);
  BOOST_TEST_EQ(num_erased, size - set.size());
}

UNORDERED_AUTO_TEST (unordered_erase_if) {
  test_map_erase_if<boost::unordered_map<std::string, int> >();
  test_map_erase_if<boost::unordered_multimap<std::string, int> >();

  test_set_erase_if<boost::unordered_set<int> >();
  test_set_erase_if<boost::unordered_multiset<int> >();
}

RUN_TESTS()
