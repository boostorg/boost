//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>

namespace gil = boost::gil;
namespace mp11 = boost::mp11;

void check_histogram_constructors()
{
    gil::histogram<int> h1;
    gil::histogram<int> h2 = h1;
    gil::histogram<int, int, int> h3;
    gil::histogram<int, int ,int> h4(h3);

    static_assert(h1.dimension() == h2.dimension(),"Dimension mismatch");
    static_assert(h1.dimension() != h3.dimension(),"Dimension mismatch");
    static_assert(h3.dimension() == h4.dimension(),"Dimension mismatch");
}

int main() {

    check_histogram_constructors();
    return 0;

}
