// Boost.Geometry
// QuickBook Example

// Copyright (c) 2021, Oracle and/or its affiliates
// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[closest_points
//` Shows calculation of the two closest points between two geometries

#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_point.hpp>

/*<-*/ #include "../examples_utils/create_svg_closest_points.hpp" /*->*/

int main()
{
    typedef boost::geometry::model::d2::point_xy<double> point_type;
    typedef boost::geometry::model::polygon<point_type> polygon_type;
    typedef boost::geometry::model::linestring<point_type> linestring_type;
    typedef boost::geometry::model::multi_point<point_type> multi_point_type;

    point_type p(4.3,1.9);
    point_type p0(0,0);
    polygon_type poly;
    linestring_type line;
    multi_point_type mp;

    boost::geometry::read_wkt(
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", poly);
    line.push_back(point_type(5,0));
    line.push_back(point_type(2,0));
    mp.push_back(point_type(2,0.5));
    mp.push_back(point_type(2,3));

    boost::geometry::model::segment<point_type> sout1,sout2,sout3;

    boost::geometry::closest_points(p, poly, sout1);
    std::cout << "Point-Poly: " << boost::geometry::wkt(sout1) << std::endl;

    boost::geometry::closest_points(poly, line, sout2);
    std::cout << "Poly-Line: " << boost::geometry::wkt(sout2) << std::endl;

    boost::geometry::closest_points(poly, mp, sout3);
    std::cout << "Poly-MultiPoint: " << boost::geometry::wkt(sout3) << std::endl;

    /*<-*/ create_svg_closest_points(p0, p, line, poly, mp, sout1, sout2, sout3); /*->*/

    return 0;
}

//]


//[closest_points_output
/*`
Output:
[pre
Point-Poly: LINESTRING(4.3 1.9,4.2 2.1)
Poly-Line: LINESTRING(2.9 0.7,2.9 0)
Poly-MultiPoint: LINESTRING(2.36923 1.05385,2 0.5)
[$img/algorithms/closest_points.png]
]
*/
//]
