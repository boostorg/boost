// Copyright (C) 2014 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/lib/optional for documentation.
//
// You are welcome to contact the author at:
//  akrzemi1@gmail.com

#include "boost/optional/optional.hpp"
#include "boost/config.hpp"
#include "boost/core/lightweight_test.hpp"

#if !defined(BOOST_NO_CXX11_HDR_UNORDERED_SET) && !defined(BOOST_OPTIONAL_CONFIG_DO_NOT_SPECIALIZE_STD_HASH)

#include <unordered_set>

void test_unordered_map()
{
  std::unordered_set<boost::optional<int> > set;
  set.insert(boost::optional<int>(1));
  set.insert(boost::optional<int>(1));


  BOOST_TEST(set.size() == 1u);
  BOOST_TEST(set.find(boost::optional<int>(1)) != set.end());
}

#else

void test_unordered_map()
{}

#endif


#if !defined(BOOST_OPTIONAL_CONFIG_DO_NOT_SPECIALIZE_STD_HASH) && !defined(BOOST_NO_CXX11_HDR_FUNCTIONAL)

void tets_hash()
{
  std::hash<boost::optional<int> > hash_int;
  boost::optional<int> oN;
  boost::optional<int> o1(1);

  BOOST_TEST(hash_int(oN) == hash_int(oN));
  BOOST_TEST(hash_int(o1) == hash_int(o1));
}

#else

void tets_hash()
{}

#endif


int main()
{
  test_unordered_map();
  tets_hash();
  return boost::report_errors();
}
