// Boost.Geometry
// QuickBook Example

// Copyright (c) 2021, Oracle and/or its affiliates
// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Code to create SVG for closest_points examples

#ifndef CREATE_SVG_CLOSEST_POINTS_HPP
#define CREATE_SVG_CLOSEST_POINTS_HPP

#include <fstream>
#include <boost/geometry/io/svg/svg_mapper.hpp>

template
<
    typename Point,
    typename Line,
    typename Polygon,
    typename Multipoint,
    typename Segment
>
void create_svg_closest_points(Point const& p0,
                               Point const& p,
                               Line const& line,
                               Polygon const& poly,
                               Multipoint const& mp,
                               Segment const& sout1,
                               Segment const& sout2,
                               Segment const& sout3)
{
    // Declare a stream and an SVG mapper
    std::ofstream svg("closest_points_example.svg");
    boost::geometry::svg_mapper<Point> mapper(svg, 400, 400);

    // Add geometries such that all these geometries fit on the map

    mapper.add(p0);
    mapper.add(p);
    mapper.add(poly);
    mapper.add(line);
    mapper.add(mp);
    mapper.add(sout1);
    mapper.add(sout2);
    mapper.add(sout3);

    // Draw the geometries on the SVG map, using a specific SVG style
    mapper.map(sout1, "opacity:1.0;fill:rgb(255,0,0);stroke:rgb(0,0,0);stroke-width:1;stroke-dasharray:1,2");
    mapper.map(sout2, "opacity:1.0;fill:rgb(255,0,0);stroke:rgb(0,0,0);stroke-width:1;stroke-dasharray:1,2");
    mapper.map(sout3, "opacity:1.0;fill:rgb(255,0,0);stroke:rgb(0,0,0);stroke-width:1;stroke-dasharray:1,2");
    mapper.map(p, "opacity:1.0;fill:rgb(255,0,0);stroke:rgb(255,0,0);stroke-width:1");
    mapper.map(poly, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
    mapper.map(line, "opacity:1.0;fill:none;stroke:rgb(0,0,255);stroke-width:2");
    mapper.map(mp, "opacity:2.0;fill:rgb(0,255,255);stroke:rgb(0,255,255);stroke-width:.5");
}

// NOTE: convert manually from svg to png using Inkscape ctrl-shift-E
// and copy png to html/img/...

#endif // CREATE_SVG_CLOSEST_POINTS_HPP
