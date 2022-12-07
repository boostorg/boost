// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Example: Custom coordinate system example

#include <iostream>

#include <boost/geometry/geometry.hpp>

// 1: declare a coordinate system. For example for Mars
//    Like for the Earth, we let the use choose between degrees or radians
//    (Unfortunately, in real life Mars has two coordinate systems:
//     http://planetarynames.wr.usgs.gov/Page/MARS/system)
template<typename DegreeOrRadian>
struct martian
{
    using units = DegreeOrRadian;
};

// 2: give it also a family
struct martian_tag;

// 3: register to which coordinate system family it belongs to
//    this must be done in namespace boost::geometry::traits
namespace boost { namespace geometry { namespace traits
{

template <typename DegreeOrRadian>
struct cs_tag<martian<DegreeOrRadian> >
{
    using type = martian_tag;
};

}}} // namespaces


// 5: not worked out. To implement a specific distance strategy for Mars,
//    e.g. with the Mars radius given by default,
//    you will have to implement (/register) several other metafunctions:
//      tag, return_type, similar_type, comparable_type,
//    and structs:
//      get_similar, get_comparable, result_from_distance
//   See e.g. .../boost/geometry/extensions/gis/geographic/strategies/andoyer.hpp

int main()
{
    using mars_point = boost::geometry::model::point
        <
            double, 2, martian<boost::geometry::degree>
        >;

    // Declare two points
    // (Source: http://nssdc.gsfc.nasa.gov/planetary/mars_mileage_guide.html)
    // (Other sources: Wiki and Google give slightly different coordinates, resulting
    //  in other distance, 20 km off)
    mars_point const viking1(-48.23, 22.54); // Viking 1 landing site in Chryse Planitia
    mars_point const pathfinder(-33.55, 19.33); // Pathfinder landing site in Ares Vallis

    // To calculate distance, declare and construct a strategy with Mars mean radius, in KM
    // (Source: http://nssdc.gsfc.nasa.gov/planetary/factsheet/marsfact.html)
    boost::geometry::strategies::distance::spherical<> const spherical(3389.5);

    double d = boost::geometry::distance(viking1, pathfinder, spherical);

    std::cout << "Distance between Viking1 and Pathfinder landing sites: "
        << d << " km" << std::endl;

    // We would get 832.616 here, same order as the 835 (rounded on 5 km) listed
    // on the mentioned site

    // The distance can be calculated more accurately by an Ellipsoidal approach,
    // giving 834.444 km
    boost::geometry::srs::spheroid<double> spheroid(3396.2, 3376.2);
    boost::geometry::strategy::distance::geographic<> const ellipsoidal(spheroid);

    d = boost::geometry::distance(viking1, pathfinder, ellipsoidal);
    std::cout << "Ellipsoidal distance: " << d << " km" << std::endl;

    return 0;
}
