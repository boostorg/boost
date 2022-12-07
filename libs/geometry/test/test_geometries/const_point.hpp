// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_TEST_TEST_GEOMETRIES_CONST_POINT_HPP
#define BOOST_GEOMETRY_TEST_TEST_GEOMETRIES_CONST_POINT_HPP

#include <boost/geometry/geometries/register/point.hpp>
#include <deque>
#include <vector>

class const_point
{
public:
    const_point(double x0, double y0) : x(x0), y(y0) {}

    double GetX() const { return x; }
    double GetY() const { return y; }
private:
    double x{0}, y{0};
};

BOOST_GEOMETRY_REGISTER_POINT_2D_CONST(const_point, double, boost::geometry::cs::cartesian, GetX(), GetY());

using ring_of_const_point = std::vector<const_point>;

// Register a vector of const_pos as a non-const-ring with const points
namespace boost { namespace geometry { namespace traits {
    template<> struct tag<ring_of_const_point> { typedef ring_tag type; };

}}}

using linestring_of_const_point = std::deque<const_point>;

// Register a vector of const_pos as a non-const-ring with const points
namespace boost { namespace geometry { namespace traits {
    template<> struct tag<linestring_of_const_point> { typedef linestring_tag type; };

}}}


#endif
