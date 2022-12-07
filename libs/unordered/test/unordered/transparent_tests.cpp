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

#include <boost/container_hash/hash.hpp>

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

template <class UnorderedMap> void test_map_transparent_count()
{
  count_reset();

  UnorderedMap map;

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(1, 1338));
  map.insert(std::make_pair(2, 1339));
  map.insert(std::make_pair(0, 1340));
  map.insert(std::make_pair(0, 1341));
  map.insert(std::make_pair(0, 1342));

  int const expected_key_count = key::count_;

  std::size_t count = 0;
  count = map.count(0);
  BOOST_TEST_EQ(count, map.size() - 2);

  count = map.count(1);
  BOOST_TEST_EQ(count, 1u);

  count = map.count(1337);
  BOOST_TEST_EQ(count, 0u);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedMap> void test_map_non_transparent_count()
{
  count_reset();

  UnorderedMap map;

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(1, 1338));
  map.insert(std::make_pair(2, 1339));
  map.insert(std::make_pair(0, 1340));
  map.insert(std::make_pair(0, 1341));
  map.insert(std::make_pair(0, 1342));

  int key_count = key::count_;

  std::size_t count = 0;
  count = map.count(0);
  ++key_count;

  BOOST_TEST_EQ(count, map.size() - 2);
  BOOST_TEST_EQ(key::count_, key_count);

  count = map.count(1);
  ++key_count;

  BOOST_TEST_EQ(count, 1u);

  count = map.count(1337);
  ++key_count;

  BOOST_TEST_EQ(count, 0u);
  BOOST_TEST_EQ(key::count_, key_count);
}

template <class UnorderedSet> void test_set_transparent_count()
{
  count_reset();

  UnorderedSet set;

  set.insert(0);
  set.insert(1);
  set.insert(2);
  set.insert(0);
  set.insert(0);
  set.insert(0);

  int const expected_key_count = key::count_;

  std::size_t count = 0;
  count = set.count(0);
  BOOST_TEST_EQ(count, set.size() - 2);

  count = set.count(1);
  BOOST_TEST_EQ(count, 1u);

  count = set.count(1337);
  BOOST_TEST_EQ(count, 0u);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedSet> void test_set_non_transparent_count()
{
  count_reset();

  UnorderedSet set;

  set.insert(0);
  set.insert(1);
  set.insert(2);
  set.insert(0);
  set.insert(0);
  set.insert(0);

  int key_count = key::count_;

  std::size_t count = 0;
  count = set.count(0);
  ++key_count;

  BOOST_TEST_EQ(count, set.size() - 2);
  BOOST_TEST_EQ(key::count_, key_count);

  count = set.count(1);
  ++key_count;

  BOOST_TEST_EQ(count, 1u);

  count = set.count(1337);
  ++key_count;

  BOOST_TEST_EQ(count, 0u);
  BOOST_TEST_EQ(key::count_, key_count);
}

template <class UnorderedMap> void test_map_transparent_find()
{
  count_reset();

  typedef typename UnorderedMap::const_iterator map_iterator;
  typedef typename UnorderedMap::value_type pair;

  UnorderedMap map;

  int n = 5;

  for (int i = 0; i < n; ++i) {
    map.insert(std::make_pair(i, i));
  }

  int const expected_key_count = key::count_;

  // explicitly test `find()` and `find() const` separately
  //

  {
    UnorderedMap& m = map;

    for (int i = 0; i < n; ++i) {
      map_iterator pos = m.find(i);
      BOOST_TEST(pos != m.end());

      pair const& p = *pos;
      int const v = p.second;

      BOOST_TEST_EQ(v, i);
    }

    BOOST_TEST_EQ(key::count_, expected_key_count);

    map_iterator pos = m.find(1337);
    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, expected_key_count);
  }

  {
    UnorderedMap const& m = map;

    for (int i = 0; i < n; ++i) {
      map_iterator pos = m.find(i);
      BOOST_TEST(pos != m.end());

      pair const& p = *pos;
      int const v = p.second;

      BOOST_TEST(v == i);
    }

    BOOST_TEST_EQ(key::count_, expected_key_count);

    map_iterator pos = m.find(1337);
    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, expected_key_count);
  }
}

template <class UnorderedMap> void test_map_non_transparent_find()
{
  count_reset();

  typedef typename UnorderedMap::const_iterator map_iterator;
  typedef typename UnorderedMap::value_type pair;

  UnorderedMap map;

  int n = 5;

  for (int i = 0; i < n; ++i) {
    map.insert(std::make_pair(i, i));
  }

  int key_count = key::count_;

  // explicitly test `find()` and `find() const` separately
  //

  {
    UnorderedMap& m = map;

    for (int i = 0; i < n; ++i) {
      map_iterator pos = m.find(i);
      BOOST_TEST(pos != m.end());

      pair const& p = *pos;
      int const v = p.second;

      BOOST_TEST_EQ(v, i);
    }
    BOOST_TEST_EQ(key::count_, n + key_count);

    map_iterator pos = m.find(1337);
    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, 1 + n + key_count);

    key_count = key::count_;
  }

  {
    UnorderedMap const& m = map;

    for (int i = 0; i < n; ++i) {
      map_iterator pos = m.find(i);
      BOOST_TEST(pos != m.end());

      pair const& p = *pos;
      int const v = p.second;

      BOOST_TEST_EQ(v, i);
    }
    BOOST_TEST_EQ(key::count_, n + key_count);

    map_iterator pos = m.find(1337);
    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, 1 + n + key_count);
  }
}

template <class UnorderedSet> void test_set_transparent_find()
{
  count_reset();

  typedef typename UnorderedSet::const_iterator set_iterator;

  UnorderedSet set;

  int n = 5;

  for (int i = 0; i < n; ++i) {
    set.insert(i);
  }

  int const expected_key_count = key::count_;

  // explicitly test `find()` and `find() const` separately
  //

  {
    UnorderedSet& m = set;

    for (int i = 0; i < n; ++i) {
      set_iterator pos = m.find(i);
      BOOST_TEST(pos != m.end());
      BOOST_TEST_EQ(*pos, i);
    }

    BOOST_TEST_EQ(key::count_, expected_key_count);

    set_iterator pos = m.find(1337);
    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, expected_key_count);
  }

  {
    UnorderedSet const& m = set;

    for (int i = 0; i < n; ++i) {
      set_iterator pos = m.find(i);
      BOOST_TEST(pos != m.end());
      BOOST_TEST_EQ(*pos, i);
    }

    BOOST_TEST_EQ(key::count_, expected_key_count);

    set_iterator pos = m.find(1337);
    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, expected_key_count);
  }
}

template <class UnorderedSet> void test_set_non_transparent_find()
{
  count_reset();

  typedef typename UnorderedSet::const_iterator set_iterator;

  UnorderedSet set;

  int n = 5;

  for (int i = 0; i < n; ++i) {
    set.insert(i);
  }

  int key_count = key::count_;

  // explicitly test `find()` and `find() const` separately
  //

  {
    UnorderedSet& m = set;

    for (int i = 0; i < n; ++i) {
      set_iterator pos = m.find(i);
      ++key_count;

      BOOST_TEST(pos != m.end());
      BOOST_TEST_EQ(*pos, i);
    }

    BOOST_TEST_EQ(key::count_, key_count);

    set_iterator pos = m.find(1337);
    ++key_count;

    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, key_count);
  }

  {
    UnorderedSet const& m = set;

    for (int i = 0; i < n; ++i) {
      set_iterator pos = m.find(i);
      ++key_count;

      BOOST_TEST(pos != m.end());
      BOOST_TEST_EQ(*pos, i);
    }

    BOOST_TEST_EQ(key::count_, key_count);

    set_iterator pos = m.find(1337);
    ++key_count;

    BOOST_TEST(pos == m.end());
    BOOST_TEST_EQ(key::count_, key_count);
  }
}

template <class UnorderedMap> void test_map_transparent_equal_range()
{
  count_reset();

  UnorderedMap unordered_map;

  // empty tests
  //
  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedMap::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap& map = unordered_map;
    BOOST_TEST(map.empty());

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  {
    typedef typename UnorderedMap::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap const& map = unordered_map;
    BOOST_TEST(map.empty());

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  BOOST_TEST_EQ(key::count_, 0);

  unordered_map.insert(std::make_pair(0, 1337));
  unordered_map.insert(std::make_pair(1, 1338));
  unordered_map.insert(std::make_pair(2, 1339));
  unordered_map.insert(std::make_pair(0, 1340));
  unordered_map.insert(std::make_pair(0, 1341));
  unordered_map.insert(std::make_pair(0, 1342));

  int const expected_key_count = key::count_;

  // do this so that multimap tests actually test a range with len > 1
  //
  int const expected_range_len = static_cast<int>(unordered_map.size() - 2);

  typedef typename UnorderedMap::value_type value_type;

  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedMap::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap& map = unordered_map;

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *pos;
      BOOST_TEST_EQ(val.first.x_, 0);
    }

    iters = map.equal_range(1);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val.first, 1);
    BOOST_TEST_EQ(val.second, 1338);

    iters = map.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, expected_key_count);
  }

  {
    typedef typename UnorderedMap::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap const& map = unordered_map;

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *begin;
      BOOST_TEST_EQ(val.first.x_, 0);
    }

    iters = map.equal_range(1);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val.first, 1);
    BOOST_TEST_EQ(val.second, 1338);

    iters = map.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, expected_key_count);
  }
}

template <class UnorderedMap> void test_map_non_transparent_equal_range()
{
  count_reset();

  UnorderedMap unordered_map;

  // empty tests
  //
  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedMap::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap& map = unordered_map;
    BOOST_TEST(map.empty());

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  {
    typedef typename UnorderedMap::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap const& map = unordered_map;
    BOOST_TEST(map.empty());

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  BOOST_TEST_EQ(key::count_, 2);

  unordered_map.insert(std::make_pair(0, 1337));
  unordered_map.insert(std::make_pair(1, 1338));
  unordered_map.insert(std::make_pair(2, 1339));
  unordered_map.insert(std::make_pair(0, 1340));
  unordered_map.insert(std::make_pair(0, 1341));
  unordered_map.insert(std::make_pair(0, 1342));

  int key_count = key::count_;

  // do this so that multimap tests actually test a range with len > 1
  //
  int const expected_range_len = static_cast<int>(unordered_map.size() - 2);

  typedef typename UnorderedMap::value_type value_type;

  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedMap::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap& map = unordered_map;

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *begin;
      BOOST_TEST_EQ(val.first.x_, 0);
    }

    iters = map.equal_range(1);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val.first, 1);
    BOOST_TEST_EQ(val.second, 1338);

    iters = map.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, 3 + key_count);
    key_count += 3;
  }

  {
    typedef typename UnorderedMap::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedMap const& map = unordered_map;

    iterator_pair iters = map.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *pos;
      BOOST_TEST_EQ(val.first.x_, 0);
    }

    iters = map.equal_range(1);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val.first, 1);
    BOOST_TEST_EQ(val.second, 1338);

    iters = map.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == map.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, 3 + key_count);
  }
}

template <class UnorderedSet> void test_set_transparent_equal_range()
{
  count_reset();

  UnorderedSet unordered_set;

  // empty tests
  //
  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedSet::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet& set = unordered_set;
    BOOST_TEST(set.empty());

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  {
    typedef typename UnorderedSet::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet const& set = unordered_set;
    BOOST_TEST(set.empty());

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  BOOST_TEST_EQ(key::count_, 0);

  unordered_set.insert(0);
  unordered_set.insert(1);
  unordered_set.insert(2);
  unordered_set.insert(0);
  unordered_set.insert(0);
  unordered_set.insert(0);

  int const expected_key_count = key::count_;

  // do this so that multiset tests actually test a range with len > 1
  //
  int const expected_range_len = static_cast<int>(unordered_set.size() - 2);

  typedef typename UnorderedSet::value_type value_type;

  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedSet::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet& set = unordered_set;

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *pos;
      BOOST_TEST_EQ(val, 0);
    }

    iters = set.equal_range(1);
    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val, 1);

    iters = set.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, expected_key_count);
  }

  {
    typedef typename UnorderedSet::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet const& set = unordered_set;

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *begin;
      BOOST_TEST_EQ(val, 0);
    }

    iters = set.equal_range(1);
    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val, 1);

    iters = set.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, expected_key_count);
  }
}

template <class UnorderedSet> void test_set_non_transparent_equal_range()
{
  count_reset();

  UnorderedSet unordered_set;

  // empty tests
  //
  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedSet::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet& set = unordered_set;
    BOOST_TEST(set.empty());

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  {
    typedef typename UnorderedSet::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet const& set = unordered_set;
    BOOST_TEST(set.empty());

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);
  }

  BOOST_TEST_EQ(key::count_, 2);

  unordered_set.insert(0);
  unordered_set.insert(1);
  unordered_set.insert(2);
  unordered_set.insert(0);
  unordered_set.insert(0);
  unordered_set.insert(0);

  int key_count = key::count_;

  // do this so that multiset tests actually test a range with len > 1
  //
  int const expected_range_len = static_cast<int>(unordered_set.size() - 2);

  typedef typename UnorderedSet::value_type value_type;

  // explicitly test `equal_range()` vs `equal_range() const`
  //
  {
    typedef typename UnorderedSet::iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet& set = unordered_set;

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *begin;
      BOOST_TEST_EQ(val, 0);
    }

    iters = set.equal_range(1);
    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val, 1);

    iters = set.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, 3 + key_count);
    key_count += 3;
  }

  {
    typedef typename UnorderedSet::const_iterator iterator;
    typedef std::pair<iterator, iterator> iterator_pair;

    UnorderedSet const& set = unordered_set;

    iterator_pair iters = set.equal_range(0);

    iterator begin = iters.first;
    iterator end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), expected_range_len);

    for (iterator pos = begin; pos != end; ++pos) {
      value_type const& val = *pos;
      BOOST_TEST_EQ(val, 0);
    }

    iters = set.equal_range(1);
    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin != end);
    BOOST_TEST(begin != set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 1);

    value_type const& val = *begin;
    BOOST_TEST_EQ(val, 1);

    iters = set.equal_range(1337);

    begin = iters.first;
    end = iters.second;

    BOOST_TEST(begin == end);
    BOOST_TEST(begin == set.end());
    BOOST_TEST_EQ(std::distance(begin, end), 0);

    BOOST_TEST_EQ(key::count_, 3 + key_count);
  }
}

template <class UnorderedMap> struct convertible_to_iterator
{
  operator typename UnorderedMap::iterator()
  {
    return typename UnorderedMap::iterator();
  }
};

template <class UnorderedMap> struct convertible_to_const_iterator
{
  operator typename UnorderedMap::const_iterator()
  {
    return typename UnorderedMap::const_iterator();
  }
};

typedef boost::unordered_map<int, int, transparent_hasher,
  transparent_key_equal>
  transparent_unordered_map;

// test that in the presence of the member function template `erase()`, we still
// invoke the correct iterator overloads when the type is implicitly convertible
//
transparent_unordered_map::iterator map_erase_overload_compile_test()
{
  convertible_to_iterator<transparent_unordered_map> c;
  transparent_unordered_map map;
  transparent_unordered_map::iterator pos = map.begin();
  pos = c;
  return map.erase(c);
}

transparent_unordered_map::const_iterator
map_erase_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_map> c;
  transparent_unordered_map map;
  transparent_unordered_map::const_iterator pos = map.begin();
  pos = c;
  return map.erase(c);
}

typedef boost::unordered_multimap<int, int, transparent_hasher,
  transparent_key_equal>
  transparent_unordered_multimap;

transparent_unordered_multimap::iterator multimap_erase_overload_compile_test()
{
  convertible_to_iterator<transparent_unordered_multimap> c;
  transparent_unordered_multimap map;
  transparent_unordered_multimap::iterator pos = map.begin();
  pos = c;
  return map.erase(c);
}

transparent_unordered_multimap::const_iterator
multimap_erase_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_multimap> c;
  transparent_unordered_multimap map;
  transparent_unordered_multimap::const_iterator pos = map.begin();
  pos = c;
  return map.erase(c);
}

template <class UnorderedMap> void test_map_transparent_erase()
{
  count_reset();

  UnorderedMap map;

  std::size_t num_erased = 0;

  num_erased = map.erase(0);
  BOOST_TEST(map.empty());
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(key::count_, 0);

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(1, 1338));
  map.insert(std::make_pair(2, 1339));
  map.insert(std::make_pair(0, 1340));
  map.insert(std::make_pair(0, 1341));
  map.insert(std::make_pair(0, 1342));

  BOOST_TEST(map.find(0) != map.end());

  int const expected_key_count = key::count_;
  std::size_t const expected_num_erased = map.size() - 2;

  num_erased = map.erase(0);
  BOOST_TEST_EQ(num_erased, expected_num_erased);
  BOOST_TEST_EQ(map.size(), 2u);
  BOOST_TEST(map.find(0) == map.end());

  num_erased = map.erase(1);
  BOOST_TEST_EQ(num_erased, 1u);
  BOOST_TEST_EQ(map.size(), 1u);
  BOOST_TEST(map.find(1) == map.end());

  num_erased = map.erase(1337);
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(map.size(), 1u);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedMap> void test_map_non_transparent_erase()
{
  count_reset();

  UnorderedMap map;

  std::size_t num_erased = 0;

  num_erased = map.erase(0);
  BOOST_TEST(map.empty());
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(key::count_, 1);

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(1, 1338));
  map.insert(std::make_pair(2, 1339));
  map.insert(std::make_pair(0, 1340));
  map.insert(std::make_pair(0, 1341));
  map.insert(std::make_pair(0, 1342));

  std::size_t const expected_num_erased = map.size() - 2;

  BOOST_TEST(map.find(0) != map.end());

  int key_count = key::count_;

  num_erased = map.erase(0);
  ++key_count;
  BOOST_TEST_EQ(key::count_, key_count);
  BOOST_TEST_EQ(num_erased, expected_num_erased);
  BOOST_TEST_EQ(map.size(), 2u);

  BOOST_TEST(map.find(0) == map.end());
  ++key_count;

  BOOST_TEST_EQ(key::count_, key_count);

  num_erased = map.erase(1);
  ++key_count;
  BOOST_TEST_EQ(num_erased, 1u);
  BOOST_TEST_EQ(map.size(), 1u);

  BOOST_TEST(map.find(1) == map.end());
  ++key_count;

  num_erased = map.erase(1337);
  ++key_count;
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(map.size(), 1u);

  BOOST_TEST_EQ(key::count_, key_count);
}

typedef boost::unordered_set<int, transparent_hasher, transparent_key_equal>
  transparent_unordered_set;

typedef boost::unordered_multiset<int, transparent_hasher,
  transparent_key_equal>
  transparent_unordered_multiset;

transparent_unordered_set::iterator set_erase_overload_compile_test()
{
  convertible_to_iterator<transparent_unordered_set> c;
  transparent_unordered_set set;
  transparent_unordered_set::iterator pos = set.begin();
  pos = c;
  return set.erase(c);
}

transparent_unordered_set::const_iterator
set_erase_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_set> c;
  transparent_unordered_set set;
  transparent_unordered_set::const_iterator pos = set.begin();
  pos = c;
  return set.erase(c);
}

transparent_unordered_multiset::iterator multiset_erase_overload_compile_test()
{
  convertible_to_iterator<transparent_unordered_multiset> c;
  transparent_unordered_multiset set;
  transparent_unordered_multiset::iterator pos = set.begin();
  pos = c;
  return set.erase(c);
}

transparent_unordered_multiset::const_iterator
multiset_erase_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_multiset> c;
  transparent_unordered_multiset set;
  transparent_unordered_multiset::const_iterator pos = set.begin();
  pos = c;
  return set.erase(c);
}

template <class UnorderedSet> void test_set_transparent_erase()
{
  count_reset();

  UnorderedSet set;

  std::size_t num_erased = 0;

  num_erased = set.erase(0);
  BOOST_TEST(set.empty());
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(key::count_, 0);

  set.insert(0);
  set.insert(1);
  set.insert(2);
  set.insert(0);
  set.insert(0);
  set.insert(0);

  BOOST_TEST(set.find(0) != set.end());

  int const expected_key_count = key::count_;
  std::size_t const expected_num_erased = set.size() - 2;

  num_erased = set.erase(0);
  BOOST_TEST_EQ(num_erased, expected_num_erased);
  BOOST_TEST_EQ(set.size(), 2u);
  BOOST_TEST(set.find(0) == set.end());

  num_erased = set.erase(1);
  BOOST_TEST_EQ(num_erased, 1u);
  BOOST_TEST_EQ(set.size(), 1u);
  BOOST_TEST(set.find(1) == set.end());

  num_erased = set.erase(1337);
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(set.size(), 1u);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedSet> void test_set_non_transparent_erase()
{
  count_reset();

  UnorderedSet set;

  std::size_t num_erased = 0;

  num_erased = set.erase(0);
  BOOST_TEST(set.empty());
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(key::count_, 1);

  set.insert(0);
  set.insert(1);
  set.insert(2);
  set.insert(0);
  set.insert(0);
  set.insert(0);

  std::size_t const expected_num_erased = set.size() - 2;

  BOOST_TEST(set.find(0) != set.end());

  int key_count = key::count_;

  num_erased = set.erase(0);
  ++key_count;
  BOOST_TEST_EQ(key::count_, key_count);
  BOOST_TEST_EQ(num_erased, expected_num_erased);
  BOOST_TEST_EQ(set.size(), 2u);

  BOOST_TEST(set.find(0) == set.end());
  ++key_count;

  BOOST_TEST_EQ(key::count_, key_count);

  num_erased = set.erase(1);
  ++key_count;
  BOOST_TEST_EQ(num_erased, 1u);
  BOOST_TEST_EQ(set.size(), 1u);

  BOOST_TEST(set.find(1) == set.end());
  ++key_count;

  num_erased = set.erase(1337);
  ++key_count;
  BOOST_TEST_EQ(num_erased, 0u);
  BOOST_TEST_EQ(set.size(), 1u);

  BOOST_TEST_EQ(key::count_, key_count);
}

// test that in the presence of the member function template `extract()`, we
// still invoke the correct iterator overloads when the type is implicitly
// convertible
//
transparent_unordered_map::node_type map_extract_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_map> c;
  transparent_unordered_map map;
  transparent_unordered_map::const_iterator pos = map.begin();
  pos = c;
  return map.extract(c);
}

transparent_unordered_multimap::node_type
multimap_extract_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_multimap> c;
  transparent_unordered_multimap map;
  transparent_unordered_multimap::const_iterator pos = map.begin();
  pos = c;
  return map.extract(c);
}

template <class UnorderedMap> void test_map_transparent_extract()
{
  typedef typename UnorderedMap::node_type node_type;
  typedef typename UnorderedMap::const_iterator const_iterator;
  typedef std::pair<const_iterator, const_iterator> const_iterator_pair;

  count_reset();

  UnorderedMap map;

  node_type nh = map.extract(0);
  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(key::count_, 0);

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(1, 1338));
  map.insert(std::make_pair(2, 1339));
  map.insert(std::make_pair(0, 1340));
  map.insert(std::make_pair(0, 1341));
  map.insert(std::make_pair(0, 1342));

  std::size_t const map_size = map.size();
  int const expected_key_count = key::count_;

  nh = map.extract(0);
  BOOST_TEST_EQ(map.size(), map_size - 1);
  BOOST_TEST_EQ(nh.key().x_, 0);

  const_iterator_pair rng = map.equal_range(0);
  for (const_iterator pos = rng.first; pos != rng.second; ++pos) {
    BOOST_TEST_NE(pos->second, nh.mapped());
  }

  nh = map.extract(1337);
  BOOST_TEST(nh.empty());

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedMap> void test_map_non_transparent_extract()
{
  typedef typename UnorderedMap::node_type node_type;
  typedef typename UnorderedMap::const_iterator const_iterator;
  typedef std::pair<const_iterator, const_iterator> const_iterator_pair;

  count_reset();

  UnorderedMap map;

  node_type nh = map.extract(0);
  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(key::count_, 1);

  map.insert(std::make_pair(0, 1337));
  map.insert(std::make_pair(1, 1338));
  map.insert(std::make_pair(2, 1339));
  map.insert(std::make_pair(0, 1340));
  map.insert(std::make_pair(0, 1341));
  map.insert(std::make_pair(0, 1342));

  std::size_t const map_size = map.size();
  int key_count = key::count_;

  nh = map.extract(0);
  ++key_count;

  BOOST_TEST_EQ(map.size(), map_size - 1);
  BOOST_TEST_EQ(nh.key().x_, 0);
  BOOST_TEST_EQ(key::count_, key_count);

  const_iterator_pair rng = map.equal_range(0);
  ++key_count;

  for (const_iterator pos = rng.first; pos != rng.second; ++pos) {
    BOOST_TEST_NE(pos->second, nh.mapped());
  }

  nh = map.extract(1337);
  ++key_count;
  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(key::count_, key_count);
}

transparent_unordered_set::node_type set_extract_overload_compile_test()
{
  convertible_to_iterator<transparent_unordered_set> c;
  transparent_unordered_set set;
  transparent_unordered_set::iterator pos = set.begin();
  pos = c;
  return set.extract(c);
}

transparent_unordered_set::node_type set_extract_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_set> c;
  transparent_unordered_set set;
  transparent_unordered_set::const_iterator pos = set.begin();
  pos = c;
  return set.extract(c);
}

transparent_unordered_multiset::node_type
multiset_extract_overload_compile_test()
{
  convertible_to_iterator<transparent_unordered_multiset> c;
  transparent_unordered_multiset set;
  transparent_unordered_multiset::iterator pos = set.begin();
  pos = c;
  return set.extract(c);
}

transparent_unordered_multiset::node_type
multiset_extract_const_overload_compile_test()
{
  convertible_to_const_iterator<transparent_unordered_multiset> c;
  transparent_unordered_multiset set;
  transparent_unordered_multiset::const_iterator pos = set.begin();
  pos = c;
  return set.extract(c);
}

template <class UnorderedSet> void test_set_transparent_extract()
{
  typedef typename UnorderedSet::node_type node_type;

  count_reset();

  UnorderedSet set;

  node_type nh = set.extract(0);
  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(key::count_, 0);

  set.insert(0);
  set.insert(1);
  set.insert(2);
  set.insert(0);
  set.insert(0);
  set.insert(0);

  std::size_t const set_size = set.size();
  int const expected_key_count = key::count_;

  std::size_t count = 0;

  nh = set.extract(0);
  count = set.count(0);
  BOOST_TEST_EQ(set.size(), set_size - 1);
  BOOST_TEST_EQ(nh.value(), 0);
  BOOST_TEST_EQ(count, set_size - 3);

  set.insert(boost::move(nh));

  nh = set.extract(1);
  count = set.count(1);
  BOOST_TEST_EQ(set.size(), set_size - 1);
  BOOST_TEST_EQ(nh.value(), 1);
  BOOST_TEST_EQ(count, 0u);

  set.insert(boost::move(nh));

  nh = set.extract(1337);
  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(set.size(), set_size);

  BOOST_TEST_EQ(key::count_, expected_key_count);
}

template <class UnorderedSet> void test_set_non_transparent_extract()
{
  typedef typename UnorderedSet::node_type node_type;

  count_reset();

  UnorderedSet set;

  node_type nh = set.extract(0);
  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(key::count_, 1);

  set.insert(0);
  set.insert(1);
  set.insert(2);
  set.insert(0);
  set.insert(0);
  set.insert(0);

  std::size_t const set_size = set.size();

  int key_count = key::count_;
  std::size_t count = 0;

  nh = set.extract(0);
  ++key_count;

  count = set.count(0);
  ++key_count;

  BOOST_TEST_EQ(set.size(), set_size - 1);
  BOOST_TEST_EQ(nh.value(), 0);
  BOOST_TEST_EQ(count, set_size - 3);

  set.insert(boost::move(nh));

  nh = set.extract(1);
  ++key_count;

  count = set.count(1);
  ++key_count;

  BOOST_TEST_EQ(set.size(), set_size - 1);
  BOOST_TEST_EQ(nh.value(), 1);
  BOOST_TEST_EQ(count, 0u);

  set.insert(boost::move(nh));

  nh = set.extract(1337);
  ++key_count;

  BOOST_TEST(nh.empty());
  BOOST_TEST_EQ(set.size(), set_size);

  BOOST_TEST_EQ(key::count_, key_count);
}

void test_unordered_map()
{
  {
    typedef boost::unordered_map<key, int, transparent_hasher,
      transparent_key_equal>
      unordered_map;

    test_map_transparent_count<unordered_map>();
    test_map_transparent_find<unordered_map>();
    test_map_transparent_equal_range<unordered_map>();
    test_map_transparent_erase<unordered_map>();
    test_map_transparent_extract<unordered_map>();
  }

  {
    // non-transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_map<key, int, hasher, key_equal> unordered_map;

    test_map_non_transparent_count<unordered_map>();
    test_map_non_transparent_find<unordered_map>();
    test_map_non_transparent_equal_range<unordered_map>();
    test_map_non_transparent_erase<unordered_map>();
    test_map_non_transparent_extract<unordered_map>();
  }

  {
    // transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_map<key, int, transparent_hasher, key_equal>
      unordered_map;

    test_map_non_transparent_count<unordered_map>();
    test_map_non_transparent_find<unordered_map>();
    test_map_non_transparent_equal_range<unordered_map>();
    test_map_non_transparent_erase<unordered_map>();
    test_map_non_transparent_extract<unordered_map>();
  }

  {
    // non-transparent Hash, transparent KeyEqual
    //
    typedef boost::unordered_map<key, int, hasher, transparent_key_equal>
      unordered_map;

    test_map_non_transparent_count<unordered_map>();
    test_map_non_transparent_find<unordered_map>();
    test_map_non_transparent_equal_range<unordered_map>();
    test_map_non_transparent_erase<unordered_map>();
    test_map_non_transparent_extract<unordered_map>();
  }
}

void test_unordered_multimap()
{
  {
    typedef boost::unordered_multimap<key, int, transparent_hasher,
      transparent_key_equal>
      unordered_multimap;

    test_map_transparent_count<unordered_multimap>();
    test_map_transparent_find<unordered_multimap>();
    test_map_transparent_equal_range<unordered_multimap>();
    test_map_transparent_erase<unordered_multimap>();
    test_map_transparent_extract<unordered_multimap>();
  }

  {
    // non-transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_multimap<key, int, hasher, key_equal>
      unordered_multimap;

    test_map_non_transparent_count<unordered_multimap>();
    test_map_non_transparent_find<unordered_multimap>();
    test_map_non_transparent_equal_range<unordered_multimap>();
    test_map_non_transparent_erase<unordered_multimap>();
    test_map_non_transparent_extract<unordered_multimap>();
  }

  {
    // transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_multimap<key, int, transparent_hasher, key_equal>
      unordered_multimap;

    test_map_non_transparent_count<unordered_multimap>();
    test_map_non_transparent_find<unordered_multimap>();
    test_map_non_transparent_equal_range<unordered_multimap>();
    test_map_non_transparent_erase<unordered_multimap>();
    test_map_non_transparent_extract<unordered_multimap>();
  }

  {
    // non-transparent Hash, transparent KeyEqual
    //
    typedef boost::unordered_multimap<key, int, hasher, transparent_key_equal>
      unordered_multimap;

    test_map_non_transparent_count<unordered_multimap>();
    test_map_non_transparent_find<unordered_multimap>();
    test_map_non_transparent_equal_range<unordered_multimap>();
    test_map_non_transparent_erase<unordered_multimap>();
    test_map_non_transparent_extract<unordered_multimap>();
  }
}

void test_unordered_set()
{
  {
    typedef boost::unordered_set<key, transparent_hasher, transparent_key_equal>
      unordered_set;

    test_set_transparent_count<unordered_set>();
    test_set_transparent_find<unordered_set>();
    test_set_transparent_erase<unordered_set>();
    test_set_transparent_equal_range<unordered_set>();
    test_set_transparent_extract<unordered_set>();
  }

  {
    // non-transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_set<key, hasher, key_equal> unordered_set;

    test_set_non_transparent_count<unordered_set>();
    test_set_non_transparent_find<unordered_set>();
    test_set_non_transparent_erase<unordered_set>();
    test_set_non_transparent_equal_range<unordered_set>();
    test_set_non_transparent_extract<unordered_set>();
  }

  {
    // transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_set<key, transparent_hasher, key_equal>
      unordered_set;

    test_set_non_transparent_count<unordered_set>();
    test_set_non_transparent_find<unordered_set>();
    test_set_non_transparent_erase<unordered_set>();
    test_set_non_transparent_equal_range<unordered_set>();
    test_set_non_transparent_extract<unordered_set>();
  }

  {
    // non-transparent Hash, transparent KeyEqual
    //
    typedef boost::unordered_set<key, hasher, transparent_key_equal>
      unordered_set;

    test_set_non_transparent_count<unordered_set>();
    test_set_non_transparent_find<unordered_set>();
    test_set_non_transparent_erase<unordered_set>();
    test_set_non_transparent_equal_range<unordered_set>();
    test_set_non_transparent_extract<unordered_set>();
  }
}

void test_unordered_multiset()
{
  {
    typedef boost::unordered_multiset<key, transparent_hasher,
      transparent_key_equal>
      unordered_set;

    test_set_transparent_count<unordered_set>();
    test_set_transparent_find<unordered_set>();
    test_set_transparent_erase<unordered_set>();
    test_set_transparent_equal_range<unordered_set>();
    test_set_transparent_extract<unordered_set>();
  }

  {
    // non-transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_multiset<key, hasher, key_equal> unordered_set;

    test_set_non_transparent_count<unordered_set>();
    test_set_non_transparent_find<unordered_set>();
    test_set_non_transparent_erase<unordered_set>();
    test_set_non_transparent_equal_range<unordered_set>();
    test_set_non_transparent_extract<unordered_set>();
  }

  {
    // transparent Hash, non-transparent KeyEqual
    //
    typedef boost::unordered_multiset<key, transparent_hasher, key_equal>
      unordered_set;

    test_set_non_transparent_count<unordered_set>();
    test_set_non_transparent_find<unordered_set>();
    test_set_non_transparent_erase<unordered_set>();
    test_set_non_transparent_equal_range<unordered_set>();
    test_set_non_transparent_extract<unordered_set>();
  }

  {
    // non-transparent Hash, transparent KeyEqual
    //
    typedef boost::unordered_multiset<key, hasher, transparent_key_equal>
      unordered_set;

    test_set_non_transparent_count<unordered_set>();
    test_set_non_transparent_find<unordered_set>();
    test_set_non_transparent_erase<unordered_set>();
    test_set_non_transparent_equal_range<unordered_set>();
    test_set_non_transparent_extract<unordered_set>();
  }
}

UNORDERED_AUTO_TEST (transparent_ops) {
  test_unordered_map();
  test_unordered_multimap();
  test_unordered_set();
  test_unordered_multiset();
}

RUN_TESTS()
