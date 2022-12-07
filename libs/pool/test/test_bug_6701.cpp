/* Copyright (C) 2012 Étienne Dupuis
*
* Use, modification and distribution is subject to the
* Boost Software License, Version 1.0. (See accompanying
* file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

// Test of bug #6701 (https://svn.boost.org/trac/boost/ticket/6701)

#include <boost/pool/object_pool.hpp>
#include <boost/limits.hpp>

int main()
{
  boost::pool<> p(1024, std::numeric_limits<size_t>::max() / 768);

  void *x = p.malloc();
  if (sizeof(size_t) > 4)
    BOOST_ASSERT(!x);

  BOOST_ASSERT(std::numeric_limits<size_t>::max() / 1024 >= p.get_next_size());
  BOOST_ASSERT(std::numeric_limits<size_t>::max() / 1024 >= p.get_max_size());

  void *y = p.ordered_malloc(std::numeric_limits<size_t>::max() / 768);
  if (sizeof(size_t) > 4)
    BOOST_ASSERT(!y);

  return 0;
}
