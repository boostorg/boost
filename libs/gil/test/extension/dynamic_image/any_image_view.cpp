//
// Copyright 2020 Samuel Debionne
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>
#include <boost/gil/extension/dynamic_image/any_image_view.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>

#include <type_traits>

#include "test_fixture.hpp"
#include "core/image/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

void test_any_image_view_nested_types()
{
    BOOST_TEST_TRAIT_SAME(gil::any_image_view<gil::gray8_view_t>::const_t, gil::any_image_view<gil::gray8c_view_t>);
}


struct test_any_image_view_init_ctor
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using view_t = typename Image::view_t;
        using any_view_t = gil::any_image_view<view_t>;
        using any_const_view_t = typename any_view_t::const_t;
        Image i0(fixture::create_image<image_t>(4, 5, 128));

        view_t v0 = gil::view(i0);
        any_view_t v1 = v0;

        BOOST_TEST_EQ(v1.dimensions().x, 4);
        BOOST_TEST_EQ(v1.dimensions().y, 5);
        BOOST_TEST_EQ(v1.width(), 4);
        BOOST_TEST_EQ(v1.height(), 5);
        BOOST_TEST_EQ(v1.size(), 4 * 5);
        BOOST_TEST_EQ(v1.num_channels(), gil::num_channels<image_t>::value);

        any_const_view_t v2 = v0;

        BOOST_TEST_EQ(v2.dimensions().x, 4);
        BOOST_TEST_EQ(v2.dimensions().y, 5);

        //any_const_view_t v3 = v1;
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_any_image_view_init_ctor{});
    }
};

struct test_any_image_view_copy_ctor
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using view_t = typename Image::view_t;
        using any_view_t = gil::any_image_view<view_t>;
        using any_const_view_t = typename any_view_t::const_t;
        Image i0(fixture::create_image<image_t>(4, 4, 128));

        view_t v0 = gil::view(i0);
        any_view_t v1 = v0;

        BOOST_TEST_EQ(v1.dimensions().x, 4);
        BOOST_TEST_EQ(v1.dimensions().y, 4);

        any_const_view_t v2 = v0;

        BOOST_TEST_EQ(v2.dimensions().x, 4);
        BOOST_TEST_EQ(v2.dimensions().y, 4);

        //any_const_view_t v3 = v1;
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_any_image_view_copy_ctor{});
    }
};

struct test_any_image_view_assign_operator
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using view_t = typename Image::view_t;
        using any_view_t = gil::any_image_view<view_t>;
        using any_const_view_t = typename any_view_t::const_t;
        Image i0(fixture::create_image<image_t>(4, 4, 128));

        view_t v0 = gil::view(i0);
        any_view_t v1;
        any_const_view_t v2;

        v1 = v0;

        BOOST_TEST_EQ(v1.dimensions().x, 4);
        BOOST_TEST_EQ(v1.dimensions().y, 4);

        v2 = v0;

        BOOST_TEST_EQ(v2.dimensions().x, 4);
        BOOST_TEST_EQ(v2.dimensions().y, 4);

        //v2 = v1;
    }
    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_any_image_view_assign_operator{});
    }
};

int main()
{
    test_any_image_view_init_ctor::run();
    test_any_image_view_copy_ctor::run();
    test_any_image_view_assign_operator::run();

    return ::boost::report_errors();
}
