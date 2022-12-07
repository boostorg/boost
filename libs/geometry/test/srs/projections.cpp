// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2018, 2022.
// Modifications copyright (c) 2018, 2022, Oracle and/or its affiliates.
// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

    // WARNING: this file takes several minutes to quarters to compile on GCC


#if defined(_MSC_VER)
#pragma warning( disable : 4305 ) // truncation double -> float
#endif // defined(_MSC_VER)


#include <sstream>

#include <geometry_test_common.hpp>

#include <boost/geometry/geometries/point_xy.hpp>

#include <boost/geometry/srs/projection.hpp>
#include <boost/geometry/srs/transformation.hpp>

#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>

#include <test_common/test_point.hpp>


namespace srs = bg::srs;

inline void check(double v, double ve, std::string const& name,
                  std::string const& axis, double tol)
{
    //BOOST_CHECK_CLOSE(v, ve, 0.001);

    bool ok = ::boost::test_tools::check_is_close_t()(v, ve, ::boost::math::fpc::percent_tolerance(tol));
    BOOST_CHECK_MESSAGE((ok), name << " " << axis << " -> " << v << " != " << ve);
}

template <typename P>
void test_forward(std::string const& name,
              double lon, double lat,
              typename bg::coordinate_type<P>::type x,
              typename bg::coordinate_type<P>::type y,
              std::string const& parameters,
              double tol = 0.001)
{
    typedef typename bg::coordinate_type<P>::type coord_type;
    typedef bg::model::point<coord_type, 2, bg::cs::geographic<bg::degree> > lonlat_type;

    lonlat_type ll;
    bg::set<0>(ll, lon);
    bg::set<1>(ll, lat);

    srs::projection<> prj = srs::proj4(parameters);

    P xy;
    prj.forward(ll, xy);

    //std::cout << std::setprecision(16) << bg::get<0>(xy) << " " << bg::get<1>(xy) << std::endl;

    check(bg::get<0>(xy), x, name, "x", tol);
    check(bg::get<1>(xy), y, name, "y", tol);
}

template <typename P>
void test_inverse(std::string const& name,
              typename bg::coordinate_type<P>::type x,
              typename bg::coordinate_type<P>::type y,
              double lon, double lat,
              std::string const& parameters,
              double tol = 0.001)
{
    typedef typename bg::coordinate_type<P>::type coord_type;
    typedef bg::model::point<coord_type, 2, bg::cs::geographic<bg::degree> > lonlat_type;

    P xy;
    bg::set<0>(xy, x);
    bg::set<1>(xy, y);

    srs::projection<> prj = srs::proj4(parameters);

    lonlat_type ll;
    prj.inverse(xy, ll);

    //std::cout << std::setprecision(16) << bg::get<0>(ll) << " " << bg::get<1>(ll) << std::endl;

    check(bg::get<0>(ll), lon, name, "lon", tol);
    check(bg::get<1>(ll), lat, name, "lat", tol);
}


template <typename P>
void test_all()
{
    test_forward<P>("aea", 4.897000, 52.371000, 334609.583974, 5218502.503686, "+proj=aea +ellps=WGS84 +units=m +lat_1=55 +lat_2=65");
    test_forward<P>("aeqd", 4.897000, 52.371000, 384537.462363, 5809944.807548, "+proj=aeqd +ellps=WGS84 +units=m");
    test_forward<P>("airy", 4.897000, 52.371000, 328249.003313, 4987937.101447, "+proj=airy +ellps=WGS84 +units=m");
    test_forward<P>("aitoff", 4.897000, 52.371000, 384096.182830, 5831239.274680, "+proj=aitoff +ellps=WGS84 +units=m");
    test_forward<P>("alsk", -84.390000, 33.755000, 7002185.416415, -3700467.546545, "+proj=alsk +ellps=WGS84 +units=m +lon_0=-150W");
    test_forward<P>("apian", 4.897000, 52.371000, -489503.746852, 5829913.052335, "+proj=apian +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_forward<P>("august", 4.897000, 52.371000, 472494.816642, 6557137.075680, "+proj=august +ellps=WGS84 +units=m");
    test_forward<P>("bacon", 4.897000, 52.371000, -276322.940590, 7934660.138798, "+proj=bacon +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_forward<P>("bipc", 4.897000, 52.371000, 3693973.674143, -8459972.647559, "+proj=bipc +ellps=WGS84 +units=m");
    test_forward<P>("boggs", 4.897000, 52.371000, -469734.523784, 5966441.169806, "+proj=boggs +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_forward<P>("bonne", 4.897000, 52.371000, 333291.091896, 274683.016972, "+proj=bonne +ellps=WGS84 +units=m +lat_1=50");
    test_forward<P>("cass", 4.897000, 52.371000, 333274.431072, 5815921.803069, "+proj=cass +ellps=WGS84 +units=m");
    test_forward<P>("cc", 4.897000, 52.371000, 545131.546415, 8273513.720038, "+proj=cc +ellps=WGS84 +units=m");
    test_forward<P>("cea", 4.897000, 52.371000, -738753.247401, 5031644.669407, "+proj=cea +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_forward<P>("chamb", 4.897000, 52.371000, -3221300.532044, 872840.127676, "+proj=chamb +ellps=WGS84 +units=m +lat_1=52 +lon_1=5 +lat_2=30 +lon_2=80 +lat_3=20 +lon_3=-50");
    test_forward<P>("col_urban", -74.25000002299999835, 4.7999999945000002555, 80859.03040773406974, 122543.17368443853047, "+proj=col_urban +lat_0=4.68048611111111 +lon_0=-74.1465916666667 +x_0=92334.879 +y_0=109320.965 +h_0=2550 +ellps=GRS80");
    test_forward<P>("collg", 4.897000, 52.371000, 280548.640940, 6148862.475491, "+proj=collg +ellps=WGS84 +units=m");
    test_forward<P>("crast", 4.897000, 52.371000, 340944.220871, 5874029.522010, "+proj=crast +ellps=WGS84 +units=m");
    test_forward<P>("denoy", 4.897000, 52.371000, 382253.324398, 5829913.052335, "+proj=denoy +ellps=WGS84 +units=m");
    test_forward<P>("eck1", 4.897000, 52.371000, 356112.818167, 5371202.270688, "+proj=eck1 +ellps=WGS84 +units=m");
    test_forward<P>("eck2", 4.897000, 52.371000, 320023.223588, 6697754.654662, "+proj=eck2 +ellps=WGS84 +units=m");
    test_forward<P>("eck3", 4.897000, 52.371000, 417367.858470, 4923223.990430, "+proj=eck3 +ellps=WGS84 +units=m");
    test_forward<P>("eck4", 4.897000, 52.371000, 383678.300021, 6304427.033917, "+proj=eck4 +ellps=WGS84 +units=m");
    test_forward<P>("eck5", 4.897000, 52.371000, 387191.346304, 5142132.228246, "+proj=eck5 +ellps=WGS84 +units=m");
    test_forward<P>("eck6", 4.897000, 52.371000, 342737.885307, 6363364.830847, "+proj=eck6 +ellps=WGS84 +units=m");
    test_forward<P>("eqc", 4.897000, 52.371000, 545131.546415, 5829913.052335, "+proj=eqc +ellps=WGS84 +units=m");
    test_forward<P>("eqdc", 4.897000, 52.371000, 307874.536263, 5810915.646438, "+proj=eqdc +ellps=WGS84 +units=m +lat_1=60 +lat_2=0");
    test_forward<P>("etmerc", 4.897000, 52.371000, 333425.492123, 5815921.814393, "+proj=etmerc +ellps=WGS84 +units=m");
    test_forward<P>("euler", 4.897000, 52.371000, 338753.024859, 5836825.984893, "+proj=euler +ellps=WGS84 +units=m +lat_1=60 +lat_2=0");
    test_forward<P>("fahey", 4.897000, 52.371000, 388824.354103, 5705638.873094, "+proj=fahey +ellps=WGS84 +units=m");
    test_forward<P>("fouc", 4.897000, 52.371000, 268017.369817, 6272855.564674, "+proj=fouc +ellps=WGS84 +units=m");
    test_forward<P>("fouc_s", 4.897000, 52.371000, 545131.546415, 5051361.531375, "+proj=fouc_s +ellps=WGS84 +units=m");
    test_forward<P>("gall", 4.897000, 52.371000, 385466.213109, 5354217.135929, "+proj=gall +ellps=WGS84 +units=m");
    test_forward<P>("geocent", 4.897000, 52.371000, 545131.546415, 5829913.052335, "+proj=geocent +ellps=WGS84 +units=m");
    test_forward<P>("geos", 4.897000, 52.371000, 313594.638994, 4711397.361812, "+proj=geos +ellps=WGS84 +units=m +h=40000000");
    test_forward<P>("gins8", 4.897000, 52.371000, 409919.989781, 6235811.415629, "+proj=gins8 +ellps=WGS84 +units=m");
    test_forward<P>("gn_sinu", 4.897000, 52.371000, 326082.668183, 6264971.711917, "+proj=gn_sinu +ellps=WGS84 +units=m +m=0.5 +n=1.785");
    test_forward<P>("gnom", 4.897000, 52.371000, 546462.815658, 8303824.612633, "+proj=gnom +ellps=WGS84 +units=m");
    test_forward<P>("goode", 4.897000, 52.371000, 360567.451176, 5782693.787691, "+proj=goode +ellps=WGS84 +units=m");
    test_forward<P>("gs48", -84.390000, 33.755000, 4904886.323054, 12421187.782392, "+proj=gs48 +ellps=WGS84 +units=m +lon1=-48");
    test_forward<P>("gs50", -84.390000, 33.755000, 3190310.148850, -564230.076744, "+proj=gs50 +ellps=WGS84 +units=m +lon1=-50");
    test_forward<P>("gstmerc", 4.897000, 52.371000, 333173.875017, 5815062.181746, "+proj=gstmerc +ellps=WGS84 +units=m");
    test_forward<P>("hammer", 4.897000, 52.371000, 370843.923425, 5630047.232233, "+proj=hammer +ellps=WGS84 +units=m");
    test_forward<P>("hatano", 4.897000, 52.371000, 383644.128560, 6290117.704632, "+proj=hatano +ellps=WGS84 +units=m");
    test_forward<P>("healpix", 4.897000, 52.371000, 1469886.5704, 6042138.5098, "+proj=healpix +ellps=WGS84 +units=m");
    test_forward<P>("rhealpix", 4.897000, 52.371000, -11477441.24814, 13972970.8457, "+proj=rhealpix +ellps=WGS84 +units=m");
    test_forward<P>("imw_p", 4.897000, 52.371000, 318784.808056, 3594184.939568, "+proj=imw_p +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n +lon_1=5");
    test_forward<P>("isea", 4.897000, 52.371000, -413613.639976, 9218173.701546, "+proj=isea +ellps=WGS84 +units=m");
    test_forward<P>("kav5", 4.897000, 52.371000, 383646.088858, 5997047.888175, "+proj=kav5 +ellps=WGS84 +units=m");
    test_forward<P>("kav7", 4.897000, 52.371000, 407769.043907, 5829913.052335, "+proj=kav7 +ellps=WGS84 +units=m");
    test_forward<P>("krovak", 14.416667, 50.083333, -743286.779768, -1043498.912060, "+proj=krovak +ellps=WGS84 +units=m");
    test_forward<P>("laea", 4.897000, 52.371000, 371541.476735, 5608007.251007, "+proj=laea +ellps=WGS84 +units=m");
    test_forward<P>("lagrng", 4.897000, 52.371000, 413379.673720, 6281547.821085, "+proj=lagrng +ellps=WGS84 +units=m +W=1");
    test_forward<P>("larr", 4.897000, 52.371000, 485541.716273, 6497324.523196, "+proj=larr +ellps=WGS84 +units=m");
    test_forward<P>("lask", 4.897000, 52.371000, 456660.618715, 6141427.377857, "+proj=lask +ellps=WGS84 +units=m");
    test_forward<P>("latlon", 4.897000, 52.371000, 0.085469, 0.914046, "+proj=latlon +ellps=WGS84 +units=m");
    test_forward<P>("latlong", 4.897000, 52.371000, 0.085469, 0.914046, "+proj=latlong +ellps=WGS84 +units=m");
    test_forward<P>("lcc", 4.897000, 52.371000, 319700.820572, 5828852.504871, "+proj=lcc +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_forward<P>("lcca", 4.897000, 52.371000, 363514.402883, 2555324.493896, "+proj=lcca +ellps=WGS84 +units=m +lat_0=30n +lat_1=55n +lat_2=60n");
    test_forward<P>("leac", 4.897000, 52.371000, 249343.870798, 6909632.226405, "+proj=leac +ellps=WGS84 +units=m");
    test_forward<P>("lee_os", -84.390000, 33.755000, 7657412.020774, 4716426.185485, "+proj=lee_os +ellps=WGS84 +units=m");
    test_forward<P>("longlat", 4.897000, 52.371000, 0.085469, 0.914046, "+proj=longlat +ellps=WGS84 +units=m");
    test_forward<P>("lonlat", 4.897000, 52.371000, 0.085469, 0.914046, "+proj=lonlat +ellps=WGS84 +units=m");
    test_forward<P>("loxim", 4.897000, 52.371000, 462770.371742, 5829913.052335, "+proj=loxim +ellps=WGS84 +units=m");
    test_forward<P>("lsat", -84.390000, 33.755000, 16342543.294793, -2092348.169198, "+proj=lsat +ellps=WGS84 +units=m +lsat=1 +path=1");
    test_forward<P>("mbt_fps", 4.897000, 52.371000, 392815.792409, 6007058.470101, "+proj=mbt_fps +ellps=WGS84 +units=m");
    test_forward<P>("mbt_s", 4.897000, 52.371000, 389224.301381, 5893467.204064, "+proj=mbt_s +ellps=WGS84 +units=m");
    test_forward<P>("mbtfpp", 4.897000, 52.371000, 345191.582111, 6098551.031494, "+proj=mbtfpp +ellps=WGS84 +units=m");
    test_forward<P>("mbtfpq", 4.897000, 52.371000, 371214.469979, 5901319.366034, "+proj=mbtfpq +ellps=WGS84 +units=m");
    test_forward<P>("mbtfps", 4.897000, 52.371000, 325952.066750, 6266156.827884, "+proj=mbtfps +ellps=WGS84 +units=m");
    test_forward<P>("merc", 4.897000, 52.371000, 545131.546415, 6833623.829215, "+proj=merc +ellps=WGS84 +units=m");
    test_forward<P>("mil_os", 4.897000, 52.371000, -1017212.552960, 3685935.358004, "+proj=mil_os +ellps=WGS84 +units=m");
    test_forward<P>("mill", 4.897000, 52.371000, 545131.546415, 6431916.372717, "+proj=mill +ellps=WGS84 +units=m");
    test_forward<P>("moll", 4.897000, 52.371000, 360567.451176, 6119459.421291, "+proj=moll +ellps=WGS84 +units=m");
    test_forward<P>("murd1", 4.897000, 52.371000, 333340.993642, 5839071.944597, "+proj=murd1 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_forward<P>("murd2", 4.897000, 52.371000, 317758.821713, 6759296.097305, "+proj=murd2 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_forward<P>("murd3", 4.897000, 52.371000, 331696.409000, 5839224.186916, "+proj=murd3 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_forward<P>("natearth", 4.897000, 52.371000, 409886.629231, 5862282.218987, "+proj=natearth +ellps=WGS84 +units=m");
    test_forward<P>("nell", 4.897000, 52.371000, 454576.246081, 5355027.851999, "+proj=nell +ellps=WGS84 +units=m");
    test_forward<P>("nell_h", 4.897000, 52.371000, 438979.742911, 5386970.539995, "+proj=nell_h +ellps=WGS84 +units=m");
    test_forward<P>("nicol", 4.897000, 52.371000, 360493.071000, 5836451.532406, "+proj=nicol +ellps=WGS84 +units=m");
    test_forward<P>("nsper", 4.897000, 52.371000, 0.521191, 7.919806, "+proj=nsper +ellps=WGS84 +units=m +a=10 +h=40000000");
    test_forward<P>("nzmg", 174.783333, -36.850000, 2669448.884228, 6482177.102194, "+proj=nzmg +ellps=WGS84 +units=m");
    test_forward<P>("ob_tran", 4.897000, 52.371000, 8688996.467740, -3348342.663884, "+proj=ob_tran +ellps=WGS84 +units=m +o_proj=moll +o_lat_p=10 +o_lon_p=90 +o_lon_o=11.50");
    test_forward<P>("ocea", 4.897000, 52.371000, 14168517.320298, -923135.204172, "+proj=ocea +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n  +lon_1=1e +lon_2=30e");
    test_forward<P>("oea", 4.897000, 52.371000, 545723.850088, 5058869.127694, "+proj=oea +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n +lon_1=1e +lon_2=30e +m=1 +n=1");
    test_forward<P>("omerc", 4.897000, 52.371000, 1009705.329154, 5829437.254923, "+proj=omerc +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n  +lon_1=1e +lon_2=30e");
    test_forward<P>("ortel", 4.897000, 52.371000, 360906.947408, 5829913.052335, "+proj=ortel +ellps=WGS84 +units=m");
    test_forward<P>("ortho", 4.897000, 52.371000, 332422.874291, 5051361.531375, "+proj=ortho +ellps=WGS84 +units=m");
    test_forward<P>("pconic", -70.400000, -23.650000, -2240096.398139, -6940342.146955, "+proj=pconic +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n +lon_0=60W");
    test_forward<P>("qsc", 4.897000, 52.371000, 543871.545186, 7341888.620371, "+proj=qsc +ellps=WGS84 +units=m");
    test_forward<P>("poly", 4.897000, 52.371000, 333274.269648, 5815908.957562, "+proj=poly +ellps=WGS84 +units=m");
    test_forward<P>("putp1", 4.897000, 52.371000, 375730.931178, 5523551.121434, "+proj=putp1 +ellps=WGS84 +units=m");
    test_forward<P>("putp2", 4.897000, 52.371000, 351480.997939, 5942668.547355, "+proj=putp2 +ellps=WGS84 +units=m");
    test_forward<P>("putp3", 4.897000, 52.371000, 287673.972013, 4651597.610600, "+proj=putp3 +ellps=WGS84 +units=m");
    test_forward<P>("putp3p", 4.897000, 52.371000, 361313.008033, 4651597.610600, "+proj=putp3p +ellps=WGS84 +units=m");
    test_forward<P>("putp4p", 4.897000, 52.371000, 351947.465829, 6330828.716145, "+proj=putp4p +ellps=WGS84 +units=m");
    test_forward<P>("putp5", 4.897000, 52.371000, 320544.316171, 5908383.682019, "+proj=putp5 +ellps=WGS84 +units=m");
    test_forward<P>("putp5p", 4.897000, 52.371000, 436506.666600, 5908383.682019, "+proj=putp5p +ellps=WGS84 +units=m");
    test_forward<P>("putp6", 4.897000, 52.371000, 324931.055842, 5842588.644796, "+proj=putp6 +ellps=WGS84 +units=m");
    test_forward<P>("putp6p", 4.897000, 52.371000, 338623.512107, 6396742.919679, "+proj=putp6p +ellps=WGS84 +units=m");
    test_forward<P>("qua_aut", 4.897000, 52.371000, 370892.621714, 5629072.862494, "+proj=qua_aut +ellps=WGS84 +units=m");
    test_forward<P>("robin", 4.897000, 52.371000, 394576.507489, 5570940.631371, "+proj=robin +ellps=WGS84 +units=m");
    test_forward<P>("rouss", 4.897000, 52.371000, 412826.227669, 6248368.849775, "+proj=rouss +ellps=WGS84 +units=m");
    test_forward<P>("rpoly", 4.897000, 52.371000, 332447.130797, 5841164.662431, "+proj=rpoly +ellps=WGS84 +units=m");
    test_forward<P>("sinu", 4.897000, 52.371000, 333528.909809, 5804625.044313, "+proj=sinu +ellps=WGS84 +units=m");
    test_forward<P>("somerc", 4.897000, 52.371000, 545131.546415, 6833623.829215, "+proj=somerc +ellps=WGS84 +units=m");
    test_forward<P>("stere", 4.897000, 52.371000, 414459.621827, 6255826.749872, "+proj=stere +ellps=WGS84 +units=m +lat_ts=30n");
    test_forward<P>("sterea", 4.897000, 52.371000, 121590.388077, 487013.903377, "+proj=sterea +lat_0=52.15616055555555 +lon_0=5.38763888888889 +k=0.9999079 +x_0=155000 +y_0=463000 +ellps=bessel +units=m");
    test_forward<P>("tcc", 4.897000, 52.371000, 332875.293370, 5841186.022551, "+proj=tcc +ellps=WGS84 +units=m");
    test_forward<P>("tcea", 4.897000, 52.371000, 332422.874291, 5841186.022551, "+proj=tcea +ellps=WGS84 +units=m");
    test_forward<P>("tissot", 4.897000, 52.371000, 431443.972539, 3808494.480735, "+proj=tissot +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_forward<P>("tmerc", 4.897000, 52.371000, 333425.492136, 5815921.814396, "+proj=tmerc +ellps=WGS84 +units=m");
    test_forward<P>("tpeqd", 4.897000, 52.371000, 998886.128891, 873800.468721, "+proj=tpeqd +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n  +lon_1=0 +lon_2=30e");
    test_forward<P>("tpers", 4.897000, 52.371000, -1172311.936260, 6263306.090352, "+proj=tpers +ellps=WGS84 +units=m +tilt=50 +azi=20 +h=40000000");
    test_forward<P>("ups", 4.897000, 52.371000, 2369508.503532, -2312783.579527, "+proj=ups +ellps=WGS84 +units=m");
    test_forward<P>("urm5", 4.897000, 52.371000, 522185.854469, 5201544.371625, "+proj=urm5 +ellps=WGS84 +units=m +n=.3 +q=.3 +alpha=10");
    test_forward<P>("urmfps", 4.897000, 52.371000, 439191.083465, 5919500.887257, "+proj=urmfps +ellps=WGS84 +units=m +n=0.50");
    test_forward<P>("utm", 4.897000, 52.371000, 220721.998929, 5810228.672907, "+proj=utm +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_forward<P>("vandg", 4.897000, 52.371000, 489005.929978, 6431581.024949, "+proj=vandg +ellps=WGS84 +units=m");
    test_forward<P>("vandg2", 4.897000, 52.371000, 488953.592205, 6434578.861895, "+proj=vandg2 +ellps=WGS84 +units=m");
    test_forward<P>("vandg3", 4.897000, 52.371000, 489028.113123, 6430309.983824, "+proj=vandg3 +ellps=WGS84 +units=m");
    test_forward<P>("vandg4", 4.897000, 52.371000, 360804.549444, 5831531.435618, "+proj=vandg4 +ellps=WGS84 +units=m");
    test_forward<P>("vitk1", 4.897000, 52.371000, 338522.044182, 5839611.656064, "+proj=vitk1 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_forward<P>("wag1", 4.897000, 52.371000, 348059.961742, 6344311.295111, "+proj=wag1 +ellps=WGS84 +units=m");
    test_forward<P>("wag2", 4.897000, 52.371000, 388567.174132, 6112322.636203, "+proj=wag2 +ellps=WGS84 +units=m");
    test_forward<P>("wag3", 4.897000, 52.371000, 447014.436776, 5829913.052335, "+proj=wag3 +ellps=WGS84 +units=m");
    test_forward<P>("wag4", 4.897000, 52.371000, 365021.547713, 6300040.998324, "+proj=wag4 +ellps=WGS84 +units=m");
    test_forward<P>("wag5", 4.897000, 52.371000, 379647.914735, 6771982.379506, "+proj=wag5 +ellps=WGS84 +units=m");
    test_forward<P>("wag6", 4.897000, 52.371000, 446107.907415, 5523551.121434, "+proj=wag6 +ellps=WGS84 +units=m");
    test_forward<P>("wag7", 4.897000, 52.371000, 366407.198644, 6169832.906560, "+proj=wag7 +ellps=WGS84 +units=m");
    test_forward<P>("webmerc", 4.897000, 52.1849, 545131.546415, 6833633.829215, "+proj=webmerc +ellps=WGS84 +units=m");
    test_forward<P>("weren", 4.897000, 52.371000, 402668.037596, 7243190.025762, "+proj=weren +ellps=WGS84 +units=m");
    test_forward<P>("wink1", 4.897000, 52.371000, 438979.742911, 5829913.052335, "+proj=wink1 +ellps=WGS84 +units=m");
    test_forward<P>("wink2", 4.897000, 52.371000, 472810.645318, 6313461.757868, "+proj=wink2 +ellps=WGS84 +units=m");
    test_forward<P>("wintri", 4.897000, 52.371000, 365568.851909, 5830576.163507, "+proj=wintri +ellps=WGS84 +units=m");

    test_inverse<P>("aea", 334609.583974, 5218502.503686, 4.897000, 52.371000, "+proj=aea +ellps=WGS84 +units=m +lat_1=55 +lat_2=65");
    test_inverse<P>("aeqd", 384537.462363, 5809944.807548, 4.897000, 52.371000, "+proj=aeqd +ellps=WGS84 +units=m"); // F/I: 883.080918
    test_inverse<P>("aitoff", 384096.182830, 5831239.274680, 4.897000, 52.371000, "+proj=aitoff +ellps=WGS84 +units=m");
    test_inverse<P>("alsk", 7002185.416415, -3700467.546545, -84.389819, 33.754911, "+proj=alsk +ellps=WGS84 +units=m +lon_0=-150W"); // F/I: 19.398478
    test_inverse<P>("bipc", 3693973.674143, -8459972.647559, 4.897000, 52.371000, "+proj=bipc +ellps=WGS84 +units=m");
    test_inverse<P>("bonne", 333291.091896, 274683.016972, 4.897000, 52.371000, "+proj=bonne +ellps=WGS84 +units=m +lat_1=50");
    test_inverse<P>("cass", 333274.431072, 5815921.803069, 4.897007, 52.371001, "+proj=cass +ellps=WGS84 +units=m"); // F/I: 0.460628
    test_inverse<P>("cc", 545131.546415, 8273513.720038, 4.897000, 52.371000, "+proj=cc +ellps=WGS84 +units=m");
    test_inverse<P>("cea", -738753.247401, 5031644.669407, 4.897000, 52.371000, "+proj=cea +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_inverse<P>("col_urban", 80859.03040773406974, 122543.17368443853047, -74.25000002299999835, 4.7999999945000002555, "+proj=col_urban +lat_0=4.68048611111111 +lon_0=-74.1465916666667 +x_0=92334.879 +y_0=109320.965 +h_0=2550 +ellps=GRS80");
    test_inverse<P>("collg", 280548.640940, 6148862.475491, 4.897000, 52.371000, "+proj=collg +ellps=WGS84 +units=m");
    test_inverse<P>("crast", 340944.220871, 5874029.522010, 4.897000, 52.371000, "+proj=crast +ellps=WGS84 +units=m");
    test_inverse<P>("eck1", 356112.818167, 5371202.270688, 4.897000, 52.371000, "+proj=eck1 +ellps=WGS84 +units=m");
    test_inverse<P>("eck2", 320023.223588, 6697754.654662, 4.897000, 52.371000, "+proj=eck2 +ellps=WGS84 +units=m");
    test_inverse<P>("eck3", 417367.858470, 4923223.990430, 4.897000, 52.371000, "+proj=eck3 +ellps=WGS84 +units=m");
    test_inverse<P>("eck4", 383678.300021, 6304427.033917, 4.897000, 52.371000, "+proj=eck4 +ellps=WGS84 +units=m");
    test_inverse<P>("eck5", 387191.346304, 5142132.228246, 4.897000, 52.371000, "+proj=eck5 +ellps=WGS84 +units=m");
    test_inverse<P>("eck6", 342737.885307, 6363364.830847, 4.897000, 52.371000, "+proj=eck6 +ellps=WGS84 +units=m");
    test_inverse<P>("eqc", 545131.546415, 5829913.052335, 4.897000, 52.371000, "+proj=eqc +ellps=WGS84 +units=m");
    test_inverse<P>("eqdc", 307874.536263, 5810915.646438, 4.897000, 52.371000, "+proj=eqdc +ellps=WGS84 +units=m +lat_1=60 +lat_2=0");
    test_inverse<P>("etmerc", 333425.492123, 5815921.814393, 4.897000, 52.371000, "+proj=etmerc +ellps=WGS84 +units=m");
    test_inverse<P>("euler", 338753.024859, 5836825.984893, 4.897000, 52.371000, "+proj=euler +ellps=WGS84 +units=m +lat_1=60 +lat_2=0");
    test_inverse<P>("fahey", 388824.354103, 5705638.873094, 4.897000, 52.371000, "+proj=fahey +ellps=WGS84 +units=m");
    test_inverse<P>("fouc", 268017.369817, 6272855.564674, 4.897000, 52.371000, "+proj=fouc +ellps=WGS84 +units=m");
    test_inverse<P>("fouc_s", 545131.546415, 5051361.531375, 4.897000, 52.371000, "+proj=fouc_s +ellps=WGS84 +units=m");
    test_inverse<P>("gall", 385466.213109, 5354217.135929, 4.897000, 52.371000, "+proj=gall +ellps=WGS84 +units=m");
    test_inverse<P>("geocent", 545131.546415, 5829913.052335, 4.897000, 52.371000, "+proj=geocent +ellps=WGS84 +units=m");
    test_inverse<P>("geos", 313594.638994, 4711397.361812, 4.897000, 52.371000, "+proj=geos +ellps=WGS84 +units=m +h=40000000");
    test_inverse<P>("gn_sinu", 326082.668183, 6264971.711917, 4.897000, 52.371000, "+proj=gn_sinu +ellps=WGS84 +units=m +m=0.5 +n=1.785");
    test_inverse<P>("gnom", 546462.815658, 8303824.612633, 4.897000, 52.371000, "+proj=gnom +ellps=WGS84 +units=m");
    test_inverse<P>("goode", 360567.451176, 5782693.787691, 4.897000, 52.371000, "+proj=goode +ellps=WGS84 +units=m");
    test_inverse<P>("gs48", 4904886.323054, 12421187.782392, -84.387019, 33.726470, "+proj=gs48 +ellps=WGS84 +units=m +lon1=-48"); // F/I: 3176.559347
    test_inverse<P>("gs50", 3190310.148850, -564230.076744, -84.389678, 33.754825, "+proj=gs50 +ellps=WGS84 +units=m +lon1=-50"); // F/I: 35.589174
    test_inverse<P>("gstmerc", 333173.875017, 5815062.181746, 4.897000, 52.371000, "+proj=gstmerc +ellps=WGS84 +units=m");
    test_inverse<P>("hatano", 383644.128560, 6290117.704632, 4.897000, 52.371000, "+proj=hatano +ellps=WGS84 +units=m");
    test_inverse<P>("healpix", 1469886.5704, 6042138.5098, 4.897000, 52.371000, "+proj=healpix +ellps=WGS84 +units=m");
    test_inverse<P>("rhealpix", -11477441.24814, 13972970.8457, 4.897000, 52.371000, "+proj=rhealpix +ellps=WGS84 +units=m");
    test_inverse<P>("imw_p", 318784.808056, 3594184.939568, 4.897000, 52.371000, "+proj=imw_p +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n +lon_1=5");
    test_inverse<P>("kav5", 383646.088858, 5997047.888175, 4.897000, 52.371000, "+proj=kav5 +ellps=WGS84 +units=m");
    test_inverse<P>("kav7", 407769.043907, 5829913.052335, 4.897000, 52.371000, "+proj=kav7 +ellps=WGS84 +units=m");
    test_inverse<P>("krovak", -743286.779768, -1043498.912060, 14.417630, 50.084517, "+proj=krovak +ellps=WGS84 +units=m"); // F/I: 148.642889
    test_inverse<P>("laea", 371541.476735, 5608007.251007, 4.897000, 52.371000, "+proj=laea +ellps=WGS84 +units=m");
    test_inverse<P>("latlon", 0.085469, 0.914046, 4.897000, 52.371000, "+proj=latlon +ellps=WGS84 +units=m");
    test_inverse<P>("latlong", 0.085469, 0.914046, 4.897000, 52.371000, "+proj=latlong +ellps=WGS84 +units=m");
    test_inverse<P>("lcc", 319700.820572, 5828852.504871, 4.897000, 52.371000, "+proj=lcc +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_inverse<P>("lcca", 363514.402883, 2555324.493896, 4.897000, 52.371000, "+proj=lcca +ellps=WGS84 +units=m +lat_0=30n +lat_1=55n +lat_2=60n");
    test_inverse<P>("leac", 249343.870798, 6909632.226405, 4.897000, 52.371000, "+proj=leac +ellps=WGS84 +units=m");
    test_inverse<P>("lee_os", 7657412.020774, 4716426.185485, -84.390000, 33.755000, "+proj=lee_os +ellps=WGS84 +units=m");
    test_inverse<P>("longlat", 0.085469, 0.914046, 4.897000, 52.371000, "+proj=longlat +ellps=WGS84 +units=m");
    test_inverse<P>("lonlat", 0.085469, 0.914046, 4.897000, 52.371000, "+proj=lonlat +ellps=WGS84 +units=m");
    test_inverse<P>("loxim", 462770.371742, 5829913.052335, 4.897000, 52.371000, "+proj=loxim +ellps=WGS84 +units=m");
    test_inverse<P>("lsat", 16342543.294793, -2092348.169198, -84.390000, 33.755000, "+proj=lsat +ellps=WGS84 +units=m +lsat=1 +path=1"); // F/I: 0.001160
    test_inverse<P>("mbt_fps", 392815.792409, 6007058.470101, 4.897000, 52.371000, "+proj=mbt_fps +ellps=WGS84 +units=m");
    test_inverse<P>("mbt_s", 389224.301381, 5893467.204064, 4.897000, 52.371000, "+proj=mbt_s +ellps=WGS84 +units=m");
    test_inverse<P>("mbtfpp", 345191.582111, 6098551.031494, 4.897000, 52.371000, "+proj=mbtfpp +ellps=WGS84 +units=m");
    test_inverse<P>("mbtfpq", 371214.469979, 5901319.366034, 4.897000, 52.371000, "+proj=mbtfpq +ellps=WGS84 +units=m");
    test_inverse<P>("mbtfps", 325952.066750, 6266156.827884, 4.897000, 52.371000, "+proj=mbtfps +ellps=WGS84 +units=m");
    test_inverse<P>("merc", 545131.546415, 6833623.829215, 4.897000, 52.371000, "+proj=merc +ellps=WGS84 +units=m");
    test_inverse<P>("mil_os", -1017212.552960, 3685935.358004, 4.897000, 52.371000, "+proj=mil_os +ellps=WGS84 +units=m");
    test_inverse<P>("mill", 545131.546415, 6431916.372717, 4.897000, 52.371000, "+proj=mill +ellps=WGS84 +units=m");
    test_inverse<P>("moll", 360567.451176, 6119459.421291, 4.897000, 52.371000, "+proj=moll +ellps=WGS84 +units=m");
    test_inverse<P>("murd1", 333340.993642, 5839071.944597, 4.897000, 52.371000, "+proj=murd1 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_inverse<P>("murd2", 317758.821713, 6759296.097305, 4.897000, 52.371000, "+proj=murd2 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_inverse<P>("murd3", 331696.409000, 5839224.186916, 4.897000, 52.371000, "+proj=murd3 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_inverse<P>("natearth", 409886.629231, 5862282.218987, 4.897000, 52.371000, "+proj=natearth +ellps=WGS84 +units=m");
    test_inverse<P>("nell", 454576.246081, 5355027.851999, 4.897000, 52.371000, "+proj=nell +ellps=WGS84 +units=m");
    test_inverse<P>("nell_h", 438979.742911, 5386970.539995, 4.897000, 52.371000, "+proj=nell_h +ellps=WGS84 +units=m");
    test_inverse<P>("nsper", 0.521191, 7.919806, 4.897000, 52.371000, "+proj=nsper +ellps=WGS84 +units=m +a=10 +h=40000000");
    test_inverse<P>("nzmg", 2669448.884228, 6482177.102194, 174.783333, -36.850000, "+proj=nzmg +ellps=WGS84 +units=m");
    test_inverse<P>("ob_tran", 8688996.467740, -3348342.663884, 4.897000, 52.371000, "+proj=ob_tran +ellps=WGS84 +units=m +o_proj=moll +o_lat_p=10 +o_lon_p=90 +o_lon_o=11.50");
    test_inverse<P>("ocea", 14168517.320298, -923135.204172, 4.897000, 52.371000, "+proj=ocea +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n  +lon_1=1e +lon_2=30e");
    test_inverse<P>("oea", 545723.850088, 5058869.127694, 4.897000, 52.371000, "+proj=oea +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n +lon_1=1e +lon_2=30e +m=1 +n=1");
    test_inverse<P>("omerc", 1009705.329154, 5829437.254923, 4.897000, 52.371000, "+proj=omerc +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n  +lon_1=1e +lon_2=30e");
    test_inverse<P>("ortho", 332422.874291, 5051361.531375, 4.897000, 52.371000, "+proj=ortho +ellps=WGS84 +units=m");
    test_inverse<P>("pconic", -2240096.398139, -6940342.146955, -70.400000, -23.650000, "+proj=pconic +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n +lon_0=60W"); // F/I: 4424863.377843
    test_inverse<P>("qsc", 543871.545186, 7341888.620371, 4.897000, 52.371000, "+proj=qsc +ellps=WGS84 +units=m");
    test_inverse<P>("poly", 333274.269648, 5815908.957562, 4.897000, 52.371000, "+proj=poly +ellps=WGS84 +units=m");
    test_inverse<P>("putp1", 375730.931178, 5523551.121434, 4.897000, 52.371000, "+proj=putp1 +ellps=WGS84 +units=m");
    test_inverse<P>("putp2", 351480.997939, 5942668.547355, 4.897000, 52.371000, "+proj=putp2 +ellps=WGS84 +units=m");
    test_inverse<P>("putp3", 287673.972013, 4651597.610600, 4.897000, 52.371000, "+proj=putp3 +ellps=WGS84 +units=m");
    test_inverse<P>("putp3p", 361313.008033, 4651597.610600, 4.897000, 52.371000, "+proj=putp3p +ellps=WGS84 +units=m");
    test_inverse<P>("putp4p", 351947.465829, 6330828.716145, 4.897000, 52.371000, "+proj=putp4p +ellps=WGS84 +units=m"); // F/I: 0.003779
    test_inverse<P>("putp5", 320544.316171, 5908383.682019, 4.897000, 52.371000, "+proj=putp5 +ellps=WGS84 +units=m");
    test_inverse<P>("putp5p", 436506.666600, 5908383.682019, 4.897000, 52.371000, "+proj=putp5p +ellps=WGS84 +units=m");
    test_inverse<P>("putp6", 324931.055842, 5842588.644796, 4.897000, 52.371000, "+proj=putp6 +ellps=WGS84 +units=m");
    test_inverse<P>("putp6p", 338623.512107, 6396742.919679, 4.897000, 52.371000, "+proj=putp6p +ellps=WGS84 +units=m");
    test_inverse<P>("qua_aut", 370892.621714, 5629072.862494, 4.897000, 52.371000, "+proj=qua_aut +ellps=WGS84 +units=m");
    test_inverse<P>("robin", 394576.507489, 5570940.631371, 4.897000, 52.371000, "+proj=robin +ellps=WGS84 +units=m");
    test_inverse<P>("rouss", 412826.227669, 6248368.849775, 4.959853, 52.433747, "+proj=rouss +ellps=WGS84 +units=m"); // F/I: 8188.459174
    test_inverse<P>("sinu", 333528.909809, 5804625.044313, 4.897000, 52.371000, "+proj=sinu +ellps=WGS84 +units=m");
    test_inverse<P>("somerc", 545131.546415, 6833623.829215, 4.897000, 52.371000, "+proj=somerc +ellps=WGS84 +units=m");
    test_inverse<P>("sterea", 121590.388077, 487013.903377, 4.897000, 52.371000, "+proj=sterea +lat_0=52.15616055555555 +lon_0=5.38763888888889 +k=0.9999079 +x_0=155000 +y_0=463000 +ellps=bessel +units=m");
    test_inverse<P>("tcea", 332422.874291, 5841186.022551, 4.897000, 52.371000, "+proj=tcea +ellps=WGS84 +units=m");
    test_inverse<P>("tissot", 431443.972539, 3808494.480735, 4.897000, 52.371000, "+proj=tissot +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_inverse<P>("tmerc", 333425.492136, 5815921.814396, 4.897000, 52.371000, "+proj=tmerc +ellps=WGS84 +units=m");
    test_inverse<P>("tpeqd", 998886.128891, 873800.468721, 4.897000, 52.371000, "+proj=tpeqd +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n  +lon_1=0 +lon_2=30e");
    test_inverse<P>("tpers", -1172311.936260, 6263306.090352, 4.897000, 52.371000, "+proj=tpers +ellps=WGS84 +units=m +tilt=50 +azi=20 +h=40000000");
    test_inverse<P>("ups", 2369508.503532, -2312783.579527, 4.897000, 52.371000, "+proj=ups +ellps=WGS84 +units=m");
    test_inverse<P>("urmfps", 439191.083465, 5919500.887257, 4.897000, 52.371000, "+proj=urmfps +ellps=WGS84 +units=m +n=0.50");
    test_inverse<P>("utm", 220721.998929, 5810228.672907, 4.897000, 52.371000, "+proj=utm +ellps=WGS84 +units=m +lon_0=11d32'00E");
    test_inverse<P>("vandg", 489005.929978, 6431581.024949, 4.897000, 52.371000, "+proj=vandg +ellps=WGS84 +units=m");
    test_inverse<P>("vitk1", 338522.044182, 5839611.656064, 4.897000, 52.371000, "+proj=vitk1 +ellps=WGS84 +units=m +lat_1=20n +lat_2=60n");
    test_inverse<P>("wag1", 348059.961742, 6344311.295111, 4.897000, 52.371000, "+proj=wag1 +ellps=WGS84 +units=m");
    test_inverse<P>("wag2", 388567.174132, 6112322.636203, 4.897000, 52.371000, "+proj=wag2 +ellps=WGS84 +units=m");
    test_inverse<P>("wag3", 447014.436776, 5829913.052335, 4.897000, 52.371000, "+proj=wag3 +ellps=WGS84 +units=m");
    test_inverse<P>("wag4", 365021.547713, 6300040.998324, 4.897000, 52.371000, "+proj=wag4 +ellps=WGS84 +units=m");
    test_inverse<P>("wag5", 379647.914735, 6771982.379506, 4.897000, 52.371000, "+proj=wag5 +ellps=WGS84 +units=m");
    test_inverse<P>("wag6", 446107.907415, 5523551.121434, 4.897000, 52.371000, "+proj=wag6 +ellps=WGS84 +units=m");
    test_inverse<P>("webmerc", 545131.546415, 6833633.829215, 4.897000, 52.1849, "+proj=webmerc +ellps=WGS84 +units=m");
    test_inverse<P>("weren", 402668.037596, 7243190.025762, 4.897000, 52.371000, "+proj=weren +ellps=WGS84 +units=m"); // F/I: 0.003779
    test_inverse<P>("wink1", 438979.742911, 5829913.052335, 4.897000, 52.371000, "+proj=wink1 +ellps=WGS84 +units=m");


    // Badly behaving inverse projections, to be reported to Gerald Evenden
    // other (probably because it is in Southern Hemisphere and inverse turns up in Northern...
    //test_inverse<P>("stere", 828919.243654, 12511653.499743, 2.183333, 41.383333, "+proj=stere +ellps=WGS84 +units=m +lat_ts=30n"); // F/I: 1238647.010132 // DIFFERENCE proj4/ggl: 4588423, (0.000000, 0.000000)
}

template <typename P>
void test_both(std::string const& name,
               double lon, double lat,
               typename bg::coordinate_type<P>::type x,
               typename bg::coordinate_type<P>::type y,
               std::string const& parameters,
               double tol = 0.001)
{
    test_forward<P>(name, lon, lat, x, y, parameters, tol);
    test_inverse<P>(name, x, y, lon, lat, parameters, tol);
}


template <typename P>
void test_srs()
{
    // Most examples from IOGP Publication 373-7-2 - Geomatics Guidance Note number 7, part 2 December 2021

    // Projection method EPSG code, method name, SRS EPSG code

    // EPSG: 1027 Lambert Azimuthal Equal Area (Spherical), 2163
    test_both<P>("laea_sph", 10, 52, 4392386.64, 5360932.79,
        "+proj=laea +a=6370997 +b=6370997 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=45 +lon_0=-100 +x_0=0 +y_0=0");

    // EPSG: 1028 Equidistant Cylindrical, 4087
    test_both<P>("eqc", 10, 52, 1113194.9079327345826 , 5788613.5212502200156,
        "+proj=eqc +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=0 +lon_0=0 +x_0=0 +y_0=0");

    // EPSG: 1029 Equidistant Cylindrical (Spherical), 4088
    test_both<P>("eqc_sph", 10, 52, 1111950.4881760599092 , 5782142.54,
        "+proj=eqc +a=6371007 +b=6371007 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=0 +lon_0=0 +x_0=0 +y_0=0");

    // EPSG: 1041 Krovak (North Orientated), 5514
    test_both<P>("krovak_north", 16.84976999999999947 , 50.209009999999999252, -568991, -1050538.64,
        "+proj=krovak +a=6377397.155 +rf=299.1528128 +towgs84=589,76,480,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=49.5111111111111 +lon_0=24.8333333333333 +x_0=0 +y_0=0 +k_0=0.9999 +alpha=78.5111111111111");
    // EPSG: 9819 Krovak 5221
    test_both<P>("krovak_north5221", 34.51644, 50.20901, -568991, -1050538.64,
        "+proj=krovak +lat_0=49.5 +lon_0=42.5 +alpha=30.28813972222222 +k=0.9999 +x_0=0 +y_0=0 +ellps=bessel +towgs84=589,76,480,0,0,0,0 +pm=ferro +units=m +no_defs");

    // EPSG: 1051 Lambert Conic Conformal (2SP Michigan) 6201
    test_both<P>("lcc2spMich", -83.166666652999992948 , 43.750000014000001158, 2308333.8, 156159.74,
        "+proj=lcc +a=6378206.4 +rf=294.9786982138982 +towgs84=-32.3841359,180.4090461,120.8442577,-2.1545854,-0.1498782,0.5742915,8.1049164 +no_defs +to_meter=0.304801 +lat_0=43.3277777777778 +lon_0=-84.3333333333333 +x_0=609601.2192024385 +y_0=0 +k_0=1.0000382 +lat_1=44.1944444444444 +lat_2=45.7");
    // EPSG: 1051 Lambert Conic Conformal (2SP Michigan) 6201
    test_both<P>("lcc2spMich", -83.166666652999992948 , 43.750000014000001158, 2308333.8, 160210,
        "+proj=lcc +a=6378206.4 +rf=294.9786982138982 +towgs84=-32.3841359,180.4090461,120.8442577,-2.1545854,-0.1498782,0.5742915,8.1049164 +no_defs +to_meter=0.304801 +lat_0=43.316666648 +lon_0=-84.333333342 +x_0=609601.2192024385 +y_0=0 +k_0=1.0000382 +lat_1=44.183333323 +lat_2=45.699999991");

    // EPSG: 1052 Colombia Urban 6247
    test_both<P>("col_urban", -74.25000002299999835 , 4.7999999945000002555, 80859.030407742538955 , 122543.1736844380066,
        "+proj=col_urban +a=6378137 +rf=298.257222101 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=4.68048611111111 +lon_0=-74.1465916666667 +x_0=92334.879 +y_0=109320.965 +h_0=2550");

    // EPSG: 9801 Lambert Conic Conformal (1SP) 24200
    test_both<P>("lcc1sp", -76.943683174000000236 , 17.93216664699999896, 255966.6, 142493.51,
        "+proj=lcc +a=6378206.4 +rf=294.9786982138982 +towgs84=-33.722,153.789,94.959,-8.581,-4.478,4.54,8.95 +no_defs +to_meter=1.000000 +lat_0=18 +lon_0=-77 +x_0=250000 +y_0=150000 +lat_1=18 +k_0=1");

    // EPSG: 9802 Lambert Conic Conformal (2SP) 32040
    test_both<P>("lcc2sp", -95.999999889999998004 , 28.500000182000000848, 2963503.95, 254759.87,
        "+proj=lcc +a=6378206.4 +rf=294.9786982138982 +towgs84=-32.3841359,180.4090461,120.8442577,-2.1545854,-0.1498782,0.5742915,8.1049164 +no_defs +to_meter=0.304801 +lat_0=27.8333333333333 +lon_0=-99 +x_0=609601.2192024385 +y_0=0 +lat_1=28.3833333333333 +lat_2=30.2833333333333");

    // EPSG: 9803 Lambert Conic Conformal (2SP Belgium) 31300
    test_both<P>("lcc2spBelg", 5.8073701499999996756 , 50.679572292000003131, 252508.5, 153048.62,
        "+proj=lcc +a=6378388 +rf=297 +towgs84=-106.8686,52.2978,-103.7239,0.3366,-0.457,1.8422,-1.2747 +no_defs +to_meter=1.000000 +lat_0=90 +lon_0=4.35693972222222 +x_0=150000.01256 +y_0=5400088.4378 +lat_1=49.8333333333333 +lat_2=51.1666666666667");

    // EPSG: 9804 Mercator (variant A) 3002
    test_both<P>("mercA", 119.99999986000001684 , -3.0000001398000000208, 5009726.57, 569150.8,
        "+proj=merc +a=6377397.155 +rf=299.1528128 +towgs84=-587.8,519.75,145.76,0,0,0,0 +no_defs +to_meter=1.000000 +lon_0=110 +x_0=3900000 +y_0=900000 +k=0.997");

    // EPSG: 9805 Mercator (variant B) 3388
    test_both<P>("mercB", 52.999999795999997332 , 52.999999795999997332, 165704.28, 5171848.04,
        "+proj=merc +a=6378245 +rf=298.3 +towgs84=25,-141,-78.5,0,0.35,0.736,0 +no_defs +to_meter=1.000000 +lon_0=51 +x_0=0 +y_0=0 +lat_ts=42");

    // EPSG: 9806 Cassini-Soldner 30200
    test_both<P>("cass", -62.000000216000003661 , 10.000000274999999661, 66644.82, 82536.37,
        "+proj=cass +a=6378293.645208759 +rf=294.26067636926103 +towgs84=-61.702,284.488,472.052,0,0,0,0 +no_defs +to_meter=0.201166 +lat_0=10.4416666666667 +lon_0=-61.3333333333333 +x_0=86501.46392052 +y_0=65379.0134283");

    // EPSG: 9807 Transverse Mercator 27700
    test_both<P>("tmerc", 0.50000021428999996687 , 50.499999870999999985, 577275, 69740.48,
        "+proj=tmerc +a=6377563.396 +rf=299.3249646 +towgs84=446.448,-125.157,542.06,0.15,0.247,0.842,-20.489 +no_defs +to_meter=1.000000 +lat_0=49 +lon_0=-2 +x_0=400000 +y_0=-100000 +k_0=0.9996012717");

    // EPSG: 9808 Transverse Mercator (South Orientated) 2053
    test_both<P>("tmercSouth", 28.282632943999999497 , -25.732028354000000547, 71984.490915335103637 , 2847342.7375604701228,
        "+proj=tmerc +axis=wsu +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=0 +lon_0=29 +x_0=0 +y_0=0 +k_0=1");

    // EPSG: 9809 Oblique Stereographic 28992
    test_both<P>("obliquesterea", 5.9999999931000003173 , 53.000000024999998516, 196105.28, 557057.74,
        "+proj=sterea +a=6377397.155 +rf=299.1528128 +towgs84=565.4171,50.3319,465.5524,-0.398957388243134,0.343987817378283,-1.87740163998045,4.0725 +no_defs +to_meter=1.000000 +lat_0=52.1561605555556 +lon_0=5.38763888888889 +x_0=155000 +y_0=463000 +k=0.9999079");

    // EPSG: 9810 Polar Stereographic (variant A) 5041
    test_both<P>("polarstereA", 44.000000006999997026 , 73.000000002999996696, 3320416.7472905800678 , 632668.43167817103676,
        "+proj=stere +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=90 +lon_0=0 +x_0=2000000 +y_0=2000000 +k_0=0.994");

    // EPSG: 9811 New Zealand Map Grid 27200
    test_both<P>("nzmg", 174.76333600000000956 , -36.848461000000000354, 2667669.23, 6482384.58,
        "+proj=nzmg +a=6378388 +rf=297 +towgs84=59.47,-5.04,187.44,0.47,-0.1,1.024,-4.5993 +no_defs +to_meter=1.000000");

    // EPSG: 9812 Hotine Oblique Mercator (variant A) 3079
    test_both<P>("omercA", 117 , 12, -4893794.4284746721387 , 12634528.92927826196,
        "+proj=omerc +no_off +a=6378137 +rf=298.257222101 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=45.3091666666667 +lonc=-86 +x_0=2546731.496 +y_0=-4354009.816 +k_0=0.9996 +alpha=337.25556 +gamma=337.25556");

    // EPSG: 9813 Laborde Oblique Mercator 8441
    test_both<P>("labrd", 44.457569999999996924 , -16.189799986000000587, 188333.59, 1100071.08,
        "+proj=labrd +a=6378388 +rf=297 +towgs84=-198.383,-240.517,-107.909,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=-18.9111111111111 +lon_0=46.4372291666667 +x_0=400000 +y_0=800000 +k_0=0.9995 +azi=18.9111111111111");

    // EPSG: 9815 Hotine Oblique Mercator (variant B) 29873
    test_both<P>("omercB", 115.80550544999999829 , 5.387253602300000388, 679245.73, 596562.78,
        "+proj=omerc +a=6377298.556 +rf=300.8017 +towgs84=-679,669,-48,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=4 +lonc=115 +x_0=590476.87 +y_0=442857.65 +k_0=0.99984 +alpha=53.3158204722222 +gamma=53.1301023611111");

    // EPSG: 9817 Lambert Conic Near-Conformal 22700
    test_both<P>("lccNear", 34.136469742000002725 , 37.521562492999997573, 15708, 623167.19,
        "+proj=lcc +a=6378249.2 +rf=293.4660212936269 +towgs84=-83.58,-397.54,458.78,-17.595,-2.847,4.256,3.225 +no_defs +to_meter=1.000000 +lat_0=34.65 +lon_0=37.35 +x_0=300000 +y_0=300000 +lat_1=34.65 +k_0=0.9996256");

    // EPSG: 9818 American Polyconic 5880
    test_both<P>("poly", -45 , -6, 5996378.7098177596927 , 9328349.9440754503012,
        "+proj=poly +a=6378137 +rf=298.257222101 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=0 +lon_0=-54 +x_0=5000000 +y_0=10000000");

    // EPSG: 9819 Krovak 5513
    test_both<P>("krovak", 16.84976999999999947 , 50.209009999999999252, 1050536.26, 568991.16,
        "+proj=krovak +czech +a=6377397.155 +rf=299.1528128 +towgs84=589,76,480,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=49.5111111111111 +lon_0=24.8333333333333 +x_0=0 +y_0=0 +k_0=0.9999");

    // EPSG: 9820 Lambert Azimuthal Equal Area 3035
    test_both<P>("laea", 5 , 50, 3962799.4509550700895 , 2999718.8531595598906,
        "+proj=laea +a=6378137 +rf=298.257222101 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=52 +lon_0=10 +x_0=4321000 +y_0=3210000");

    // EPSG: 9822 Albers Equal Area 3174
    test_both<P>("aea", -78.75 , 42.749999987000002477, 1466492.3057632399723 , 702903.12208127905615,
        "+proj=aea +a=6378137 +rf=298.257222101 +towgs84=1,1,-1,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=45.568977 +lon_0=-84.455955 +x_0=1000000 +y_0=1000000 +lat_1=42.122774 +lat_2=49.01518");

    // EPSG: 9824 Transverse Mercator Zoned Grid System 32600
    test_both<P>("utm", 12 , 56, 1798179.0365446016658 , 13588963.310720724985,
        "+proj=utm +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +zone=6");
    // EPSG: 9824 32700
    test_both<P>("utmSouth", 174 , -44, -2617060.1631802432239 , 4328084.4894244493917,
        "+proj=utm +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +zone=6 +south");

    // EPSG: 9828 Bonne (South Orientated) 5017
    test_both<P>("bonne", -9.142685 , 38.736946, 87872.79, -3183241.02,
        "+proj=bonne +lat_1=10 +axis=wsu +a=6377397.155 +rf=299.1528128 +towgs84=631.392,-66.551,481.442,1.09,-4.445,-4.487,-4.43 +no_defs +to_meter=1.000000 +lat_0=39.6777777777778 +lon_0=-8.13190611111111 +x_0=0 +y_0=0");

    // EPSG: 9829 Polar Stereographic (variant B) 3032
    test_both<P>("polarstereB", 119.99999999999998579 , -75, 7255380.7932583903894 , 7053389.5606101602316,
        "+proj=stere +lat_0=-90 +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lon_0=70 +x_0=6000000 +y_0=6000000 +lat_ts=-71");

    // EPSG: 9830 Polar Stereographic (variant C) 2985
    test_both<P>("laea", 140.07140000999999074 , -66.605227791000004345, 303169.52, 244055.72,
        "+proj=stere +lat_0=-90 +variant_c +a=6378388 +rf=297 +towgs84=365,194,166,0,0,0,0 +no_defs +to_meter=1.000000 +lon_0=140 +x_0=300000 +y_0=200000 +lat_ts=-67");

    // EPSG: 9831 Guam Projection 3993
    test_both<P>("Guam", 144.63533130999999798 , 13.339038450000000324, 37712.48, 35242,
        "+proj=aeqd +guam +a=6378206.4 +rf=294.9786982138982 +towgs84=-100,-248,259,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=13.4724663527778 +lon_0=144.748750705556 +x_0=50000 +y_0=50000");

    // EPSG: 9832 Modified Azimuthal Equidistant 3295
    test_both<P>("AzimuthEq", 138.19302999999999315 , 9.5965258593999998027, 42665.91, 65509.83,
        "+proj=aeqd +a=6378206.4 +rf=294.9786982138982 +towgs84=-100,-248,259,0,0,0,0 +no_defs +to_meter=1.000000 +lat_0=9.54670833333333 +lon_0=138.168744444444 +x_0=40000 +y_0=60000");

    // EPSG: 9833 Hyperbolic Cassini-Soldner 3139 (data from mysql 8.0.31 retrieved from epsg )
    test_both<P>("cass-hyperbolic", 179.99433651000001078 , -16.841456514000000766, 1595642.29, 1343096.93,
        "+proj=cass +hyperbolic +a=6378306.3696 +rf=293.46630765562986 +towgs84=51,391,-36,0,0,0,0 +no_defs +to_meter=0.201168 +lat_0=-16.2611111111111 +lon_0=179.344444444444 +x_0=251727.91554240003 +y_0=334519.953768");
    // EPSG: 9833 Hyperbolic Cassini-Soldner 3139
    test_both<P>("cass-hyperbolic", 179.99433651000001078 , -16.841456514000000766, 1601528.9, 1336966.01,
        "+proj=cass +hyperbolic +a=6378306.3696 +rf=293.46630765562986 +towgs84=51,391,-36,0,0,0,0 +no_defs +to_meter=0.201168 +lat_0=-16.249999974 +lon_0=179.33333332 +x_0=251727.91554240003 +y_0=334519.953768");

    // EPSG: 9834 Lambert Cylindrical Equal Area (Spherical) 3410
    test_both<P>("cea_sph", 10 , 52, 963010.77, 5797285.11,
        "+proj=cea +a=6371228 +b=6371228 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lon_0=0 +x_0=0 +y_0=0 +lat_ts=30");

    // EPSG: 9835 Lambert Cylindrical Equal Area 6933
    test_both<P>("lambert_cyl_eq_area", 10 , 52, 964862.80250896397047 , 5775916.830744350329,
        "+proj=cea +a=6378137 +rf=298.257223563 +towgs84=0,0,0,0,0,0,0 +no_defs +to_meter=1.000000 +lon_0=0 +x_0=0 +y_0=0 +lat_ts=30");

    // test +axis argument
    test_both<P>("cass", 179.99433651, -16.841456514, -322174, 268950,
        "+proj=cass +hyperbolic +lat_0=-16.25 +lon_0=179.33333332 +x_0=251727.9155424\
         +y_0=334519.953768 +towgs84=51,391,-36,0,0,0,0 +axis=wnu");
    test_both<P>("cass", 179.99433651, -16.841456514, 322174, -268950,
        "+proj=cass +hyperbolic +lat_0=-16.25 +lon_0=179.33333332 +x_0=251727.9155424\
         +y_0=334519.953768 +towgs84=51,391,-36,0,0,0,0 +axis=esu");
    test_both<P>("cass", 179.99433651, -16.841456514, -322174, -268950,
        "+proj=cass +hyperbolic +lat_0=-16.25 +lon_0=179.33333332 +x_0=251727.9155424\
         +y_0=334519.953768 +towgs84=51,391,-36,0,0,0,0 +axis=wsu");
    test_both<P>("cass", 179.99433651, -16.841456514, 268950, 322174,
        "+proj=cass +hyperbolic +lat_0=-16.25 +lon_0=179.33333332 +x_0=251727.9155424\
         +y_0=334519.953768 +towgs84=51,391,-36,0,0,0,0 +axis=neu");
}

int test_main(int, char* [])
{
    //test_all<int[2]>();
    //test_all<float[2]>();
    //test_all<double[2]>();
    //test_all<test::test_point>();
    //test_all<bg::model::d2::point_xy<int> >();
    //test_all<bg::model::d2::point_xy<float> >();
    //test_all<bg::model::d2::point_xy<double> >();

    // Leave only one here, because this divides compilation time with 6 or 7
    test_all<bg::model::d2::point_xy<long double> >();

    test_srs<bg::model::d2::point_xy<long double> >();

    return 0;
}
