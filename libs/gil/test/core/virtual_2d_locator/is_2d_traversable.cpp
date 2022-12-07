//
// Copyright 2022 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>

#include <boost/core/lightweight_test.hpp>

#include "test_fixture.hpp"
#include "core/pixel/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

struct test_pixel_types
{
    template <typename Pixel>
    void operator()(Pixel const&)
    {
        using deref_t = fixture::noop_dereference_fn<Pixel>;
        using locator_t = gil::virtual_2d_locator<deref_t, false>;
        using view_t = gil::image_view<locator_t>;

        view_t view;
        BOOST_TEST(!view.is_1d_traversable());
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::pixel_types>(test_pixel_types{});
    }
};

int main()
{
    test_pixel_types::run();

    return boost::report_errors();
}
