#include <boost/core/lightweight_test.hpp>
#include <boost/histogram.hpp>
#include <vector>
#include "throw_exception.hpp"

namespace bh = boost::histogram;

struct empty {
  int index(double) { return 0; }
  int size() const { return 0; }
};

int main() {
  {
    auto h =
        bh::make_histogram_with(std::vector<int>(), bh::axis::integer<>(0, 2), empty());

    auto ind1 = bh::indexed(h, bh::coverage::all);
    BOOST_TEST_EQ(std::distance(ind1.begin(), ind1.end()), 0);

    auto ind2 = bh::indexed(h, bh::coverage::inner);
    BOOST_TEST_EQ(std::distance(ind2.begin(), ind2.end()), 0);
  }

  {
    auto h = bh::make_histogram_with(std::vector<int>(), bh::axis::integer<>(0, 2),
                                     bh::axis::integer<>(0, 1));

    auto ind1 = bh::indexed(h, bh::coverage::all);
    BOOST_TEST_EQ(std::distance(ind1.begin(), ind1.end()), 12);

    auto ind2 = bh::indexed(h, bh::coverage::inner);
    BOOST_TEST_EQ(std::distance(ind2.begin(), ind2.end()), 2);
  }

  {
    auto h = bh::make_histogram_with(std::vector<int>(), bh::axis::integer<>(0, 2),
                                     bh::axis::integer<>(0, 0));

    auto ind1 = bh::indexed(h, bh::coverage::all);
    BOOST_TEST_EQ(std::distance(ind1.begin(), ind1.end()), 8);

    auto ind2 = bh::indexed(h, bh::coverage::inner);
    BOOST_TEST_EQ(std::distance(ind2.begin(), ind2.end()), 0);
  }

  return boost::report_errors();
}