// Copyright 2021 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// clang-format off
#include "../helpers/prefix.hpp"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../helpers/postfix.hpp"
// clang-format on

#include "../helpers/test.hpp"

struct key
{
  int x_;
  static int count_;

  key(int x) : x_(x) { ++count_; }
  key(key const& k) : x_(k.x_) { ++count_; }
};

int key::count_;

std::ostream& operator<<(std::ostream& os, key const& k)
{
  os << "key { x_: " << k.x_ << " }";
  return os;
}

bool operator==(key const& k, int const x) { return k.x_ == x; }
bool operator==(int const x, key const& k) { return k.x_ == x; }

struct transparent_hasher
{
  typedef void is_transparent;

  std::size_t operator()(key const& k) const
  {
    return boost::hash<int>()(k.x_);
  }

  std::size_t operator()(int const k) const { return boost::hash<int>()(k); }
};

struct transparent_key_equal
{
  typedef void is_transparent;

  bool operator()(key const& k1, key const& k2) const { return k1.x_ == k2.x_; }
  bool operator()(int const x, key const& k1) const { return k1 == x; }
  bool operator()(key const& k1, int const x) const { return k1 == x; }
};

struct hasher
{
  std::size_t operator()(key const& k) const
  {
    return boost::hash<int>()(k.x_);
  }
};

struct key_equal
{
  bool operator()(key const& k1, key const& k2) const { return k1.x_ == k2.x_; }
};

void count_reset() { key::count_ = 0; }

template <class UnorderedMap> void test_map_transparent_contains()
{
  count_reset();

  UnorderedMap map;
  bool contains = map.contains(0);
  BOOST_TEST(!contains);

  BOOST_TEST_EQ(key::count_, 0);

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(0, 1338));
  map.insert(std::make_pair(0, 1339));
  map.insert(std::make_pair(1, 1340));

  int const expected_key_count = key::count_;

  contains = map.contains(0);
  BOOST_TEST(contains);

  contains = map.contains(1);
  BOOST_TEST(contains);

  contains = map.contains(2);
  BOOST_TEST(!contains);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedMap> void test_map_non_transparent_contains()
{
  count_reset();

  UnorderedMap map;
  bool contains = map.contains(0);
  BOOST_TEST(!contains);
  BOOST_TEST_EQ(key::count_, 1);

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(0, 1338));
  map.insert(std::make_pair(0, 1339));
  map.insert(std::make_pair(1, 1340));

  int key_count = key::count_;

  contains = map.contains(0);
  ++key_count;
  BOOST_TEST(contains);

  contains = map.contains(1);
  ++key_count;
  BOOST_TEST(contains);

  contains = map.contains(2);
  ++key_count;
  BOOST_TEST(!contains);

  BOOST_TEST_EQ(key::count_, key_count);
}

void test_map()
{
  typedef boost::unordered_map<key, int, transparent_hasher,
    transparent_key_equal>
    transparent_map;

  typedef boost::unordered_map<key, int, transparent_hasher, key_equal>
    non_transparent_map1;

  typedef boost::unordered_map<key, int, hasher, transparent_key_equal>
    non_transparent_map2;

  typedef boost::unordered_map<key, int, hasher, key_equal>
    non_transparent_map3;

  test_map_transparent_contains<transparent_map>();
  test_map_non_transparent_contains<non_transparent_map1>();
  test_map_non_transparent_contains<non_transparent_map2>();
  test_map_non_transparent_contains<non_transparent_map3>();
}

void test_multimap()
{
  typedef boost::unordered_multimap<key, int, transparent_hasher,
    transparent_key_equal>
    transparent_multimap;

  typedef boost::unordered_multimap<key, int, transparent_hasher, key_equal>
    non_transparent_multimap1;

  typedef boost::unordered_multimap<key, int, hasher, transparent_key_equal>
    non_transparent_multimap2;

  typedef boost::unordered_multimap<key, int, hasher, key_equal>
    non_transparent_multimap3;

  test_map_transparent_contains<transparent_multimap>();
  test_map_non_transparent_contains<non_transparent_multimap1>();
  test_map_non_transparent_contains<non_transparent_multimap2>();
  test_map_non_transparent_contains<non_transparent_multimap3>();
}

template <class UnorderedSet> void test_set_transparent_contains()
{
  count_reset();

  UnorderedSet set;
  bool contains = set.contains(0);
  BOOST_TEST(!contains);

  BOOST_TEST_EQ(key::count_, 0);

  set.insert(0);
  set.insert(0);
  set.insert(0);
  set.insert(1);

  int const expected_key_count = key::count_;

  contains = set.contains(0);
  BOOST_TEST(contains);

  contains = set.contains(1);
  BOOST_TEST(contains);

  contains = set.contains(2);
  BOOST_TEST(!contains);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedSet> void test_set_non_transparent_contains()
{
  count_reset();

  UnorderedSet set;
  bool contains = set.contains(0);
  BOOST_TEST(!contains);
  BOOST_TEST_EQ(key::count_, 1);

  set.insert(0);
  set.insert(0);
  set.insert(0);
  set.insert(1);

  int key_count = key::count_;

  contains = set.contains(0);
  ++key_count;
  BOOST_TEST(contains);

  contains = set.contains(1);
  ++key_count;
  BOOST_TEST(contains);

  contains = set.contains(2);
  ++key_count;
  BOOST_TEST(!contains);

  BOOST_TEST_EQ(key::count_, key_count);
}

void test_set()
{
  typedef boost::unordered_set<key, transparent_hasher, transparent_key_equal>
    transparent_set;

  typedef boost::unordered_set<key, transparent_hasher, key_equal>
    non_transparent_set1;
  typedef boost::unordered_set<key, hasher, transparent_key_equal>
    non_transparent_set2;
  typedef boost::unordered_set<key, hasher, key_equal> non_transparent_set3;

  test_set_transparent_contains<transparent_set>();
  test_set_non_transparent_contains<non_transparent_set1>();
  test_set_non_transparent_contains<non_transparent_set2>();
  test_set_non_transparent_contains<non_transparent_set3>();
}

void test_multiset()
{
  typedef boost::unordered_multiset<key, transparent_hasher,
    transparent_key_equal>
    transparent_multiset;

  typedef boost::unordered_multiset<key, transparent_hasher, key_equal>
    non_transparent_multiset1;
  typedef boost::unordered_multiset<key, hasher, transparent_key_equal>
    non_transparent_multiset2;
  typedef boost::unordered_multiset<key, hasher, key_equal>
    non_transparent_multiset3;

  test_set_transparent_contains<transparent_multiset>();
  test_set_non_transparent_contains<non_transparent_multiset1>();
  test_set_non_transparent_contains<non_transparent_multiset2>();
  test_set_non_transparent_contains<non_transparent_multiset3>();
}

UNORDERED_AUTO_TEST (contains_) { // avoid -Wshadow warning with `bool contains`
  test_map();
  test_multimap();
  test_set();
}

RUN_TESTS()
