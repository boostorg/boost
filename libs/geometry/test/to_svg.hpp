// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2014-2021.
// Modifications copyright (c) 2014-2021 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_TEST_TO_SVG_HPP
#define BOOST_GEOMETRY_TEST_TO_SVG_HPP

#include <fstream>

#include <boost/geometry/algorithms/detail/overlay/get_turns.hpp>
#include <boost/geometry/algorithms/detail/overlay/self_turn_points.hpp>
#include <boost/geometry/algorithms/detail/overlay/traversal_info.hpp>
#include <boost/geometry/algorithms/detail/overlay/debug_turn_info.hpp>
#include <boost/geometry/algorithms/detail/overlay/enrichment_info.hpp>
#include <boost/geometry/algorithms/detail/overlay/enrich_intersection_points.hpp>
#include <boost/geometry/algorithms/detail/relate/turns.hpp>

#include <boost/geometry/io/svg/svg_mapper.hpp>
#include <boost/geometry/io/wkt/read.hpp>

#include <string_from_type.hpp>

template <typename Turns, typename Mapper>
inline void turns_to_svg(Turns const& turns, Mapper& mapper)
{
    namespace bg = boost::geometry;

    // Simple map to avoid two texts at same place (note that can still overlap!)
    std::map<std::pair<int, int>, int> offsets;
    int index = 0;
    int const margin = 5;

    for (auto const& turn : turns)
    {
        using coordinate_type
            = typename bg::coordinate_type<decltype(turn.point)>::type;
        int lineheight = 10;
        mapper.map(turn.point, "fill:rgb(255,128,0);"
                               "stroke:rgb(0,0,0);stroke-width:1", 3);

        {
            coordinate_type half = 0.5;
            coordinate_type ten = 10;
            // Map characteristics
            // Create a rounded off point
            std::pair<int, int> p
                    = std::make_pair(
                          boost::numeric_cast<int>(half
                                                   + ten * bg::get<0>(turn.point)),
                          boost::numeric_cast<int>(half
                                                   + ten * bg::get<1>(turn.point))
                          );
            std::string style =  "fill:rgb(0,0,0);font-family:Arial;font-size:12px";

            if (turn.discarded)
            {
                style =  "fill:rgb(92,92,92);font-family:Arial;font-size:10px";
                lineheight = 6;
            }

            {
                std::ostringstream out;
                out << index
                    << ": " << bg::method_char(turn.method)
                    << (turn.discarded ? " (discarded)" : "")
                    << (turn.blocked() ? " (blocked)" : "")
                    << '\n';

                out << bg::operation_char(turn.operations[0].operation)
                        <<": seg: " << turn.operations[0].seg_id.source_index
                       << ' ' << turn.operations[0].seg_id.multi_index
                       << ' ' << turn.operations[0].seg_id.ring_index
                       << ' ' << turn.operations[0].seg_id.segment_index << ", ";
                out << "other: " << turn.operations[1].seg_id.source_index
                    << ' ' << turn.operations[1].seg_id.multi_index
                    << ' ' << turn.operations[1].seg_id.ring_index
                    << ' ' << turn.operations[1].seg_id.segment_index;

                out << '\n';

                out << bg::operation_char(turn.operations[1].operation)
                        << ": seg: " << turn.operations[1].seg_id.source_index
                        << ' ' << turn.operations[1].seg_id.multi_index
                        << ' ' << turn.operations[1].seg_id.ring_index
                        << ' ' << turn.operations[1].seg_id.segment_index << ", ";
                out << "other: " << turn.operations[0].seg_id.source_index
                    << ' ' << turn.operations[0].seg_id.multi_index
                    << ' ' << turn.operations[0].seg_id.ring_index
                    << ' ' << turn.operations[0].seg_id.segment_index;


                offsets[p] += lineheight;
                int offset = offsets[p];
                offsets[p] += lineheight * 3;
                mapper.text(turn.point, out.str(), style, margin, offset, lineheight);
            }
            index++;
        }
    }
}

struct to_svg_assign_policy
{
    static bool const include_no_turn = false;
    static bool const include_degenerate = false;
    static bool const include_opposite = false;
    static bool const include_start_turn = false;
};

template <typename G1, typename G2, typename G3>
inline void to_svg(G1 const& g1, G2 const& g2, G3 const& g3,
                   std::string const& caseid)
{
    namespace bg = boost::geometry;

    using point_type = typename bg::point_type<G1>::type;
    using coordinate_type = typename bg::coordinate_type<point_type>::type;

    std::ostringstream filename;
    filename << "case_"
        << caseid << "_"
        << string_from_type<coordinate_type>::name()
#if defined(BOOST_GEOMETRY_USE_RESCALING)
        << "_rescaled"
#endif
        << ".svg";

    std::ofstream svg(filename.str());

    bg::svg_mapper<point_type> mapper(svg, 500, 500);

    mapper.add(g1);
    mapper.add(g2);
    mapper.add(g3);

    std::string const green = "rgb(153,204,0)";
    std::string const blue = "rgb(51,51,153)";
    std::string const red = "rgb(255,0,0)";
    if (bg::util::is_linestring<G1>::value)
    {
        auto style = [](auto const fo, auto const& c)
            { return "stroke:" + c + ";stroke-width:3;stroke-opacity:"
                    + std::to_string(fo) + ";"; };
        mapper.map(g1, style(0.5, green));
        mapper.map(g2, style(0.3, blue));
        mapper.map(g3, style(0.5, red));
    }
    else
    {
        auto style = [](auto const fo, auto const& c)
            { return "fill-opacity:" + std::to_string(fo) + ";fill:" + c
              + ";stroke:rgb(153,204,0);stroke-width:1;stroke-opacity:0.7;"; };
        mapper.map(g1, style(0.5, green));
        mapper.map(g2, style(0.3, blue));
        mapper.map(g3, style(0.5, red));
    }

    // GET TURNS
    
    using strategy_type
        = typename bg::strategies::relate::services::default_strategy
            <G1, G2>::type;

    using ratio_type = bg::segment_ratio<coordinate_type>;

    using turn_type = bg::detail::overlay::turn_info
        <
            point_type,
            ratio_type,
            bg::detail::overlay::turn_operation_linear<point_type, ratio_type>
        >;

    strategy_type strategy;

    std::deque<turn_type> turns;
    bg::detail::get_turns::no_interrupt_policy interrupt_policy;

    using turn_policy = bg::detail::get_turns::get_turn_info_type
        <
            G1, G2, to_svg_assign_policy
        >;

    bg::detail::relate::turns::get_turns
        <
            G1, G2, turn_policy
        >::apply(turns, g1, g2, interrupt_policy, strategy);
    {
        using less = bg::detail::relate::turns::less
            <
                0,
                bg::detail::relate::turns::less_op_xxx_linear
                    <
                        0, bg::detail::relate::turns::op_to_int<>
                    >,
                strategy_type
            >;
        std::sort(boost::begin(turns), boost::end(turns), less());
    }

    turns_to_svg(turns, mapper);
}

#endif // BOOST_GEOMETRY_TEST_TO_SVG_HPP
