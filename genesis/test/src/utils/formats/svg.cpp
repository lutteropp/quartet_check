/*
    Genesis - A toolkit for working with phylogenetic data.
    Copyright (C) 2014-2018 Lucas Czech and HITS gGmbH

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact:
    Lucas Czech <lucas.czech@h-its.org>
    Exelixis Lab, Heidelberg Institute for Theoretical Studies
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/

/**
 * @brief
 *
 * @file
 * @ingroup test
 */

#include "src/common.hpp"

#include "genesis/utils/core/fs.hpp"
#include "genesis/utils/formats/svg/svg.hpp"
#include "genesis/utils/tools/color/functions.hpp"
#include "genesis/utils/tools/color/diverging_lists.hpp"
#include "genesis/utils/tools/color/sequential_lists.hpp"
#include "genesis/utils/tools/color/map.hpp"
#include "genesis/utils/tools/color/norm_diverging.hpp"
#include "genesis/utils/tools/color/norm_boundary.hpp"

using namespace genesis::utils;

// -------------------------------------------------------------------------
//     SVG Drawing
// -------------------------------------------------------------------------

TEST( Svg, Basics )
{
    auto doc = SvgDocument();

    auto line = SvgLine( 0, 0, 100, 100 );
    line.stroke.color = color_from_bytes(128, 192, 255);
    line.stroke.width = 3.0;
    doc.add( line );

    auto rect = SvgRect( 20, 20, 60, 60 );
    rect.stroke.color = color_from_bytes( 192, 128, 0 );
    rect.fill.color = color_from_bytes( 255, 192, 0 );
    rect.fill.color.a( 0.3 );
    doc << rect;

    auto circle = SvgCircle( 80, 80, 10 );
    circle.stroke.color = color_from_bytes( 128, 255, 0 );
    circle.fill.color = color_from_bytes( 192, 255, 128);
    circle.fill.color.a( 0.5 );
    doc << circle;

    auto ellipse = SvgEllipse( 80, 20, 20, 10 );
    ellipse.stroke.color = color_from_bytes( 192, 128, 0 );
    ellipse.fill.color = color_from_bytes( 255, 192, 128);
    ellipse.fill.color.a( 0.8 );
    doc << ellipse;

    auto poly = SvgPolygon();
    poly.stroke.color = color_from_bytes( 255, 192, 0 );
    poly.fill.color = color_from_bytes( 255, 255, 0);
    poly.fill.color.a( 0.6 );
    poly << SvgPoint( 40, 00 ) << SvgPoint( 50, 20 ) << SvgPoint( 70, 10 ) << SvgPoint( 60, 30 );
    poly << SvgPoint( 80, 40 ) << SvgPoint( 60, 50 ) << SvgPoint( 70, 70 ) << SvgPoint( 50, 60 );
    poly << SvgPoint( 40, 80 ) << SvgPoint( 30, 60 ) << SvgPoint( 10, 70 ) << SvgPoint( 20, 50 );
    poly << SvgPoint( 00, 40 ) << SvgPoint( 20, 30 ) << SvgPoint( 10, 10 ) << SvgPoint( 30, 20 );
    doc << poly;

    auto text = SvgText( "Hello World! ygp", SvgPoint( 20, 120 ), SvgFont( 15 ) );
    auto bb = text.bounding_box();
    doc << SvgRect( bb.top_left, bb.size(), SvgStroke( color_from_bytes( 255, 128, 128 ) ), SvgFill( Color() ));
    doc << text;

    // doc << SvgLine( 20, 120, 20 + 12.0*10.0/2.0, 120 );
    // doc << SvgLine( 20, 120, 20, 120 - 10.0/1.2 );

    doc.margin = SvgMargin( 10, 30 );

    std::ostringstream out;
    doc.write( out );

    // LOG_DBG << out.str();
    // file_write( out.str(), "/home/lucas/test.svg" );
}

TEST( Svg, Gradient )
{
    auto doc = SvgDocument();

    auto grad = SvgGradientLinear( "bpb", SvgPoint( 0.0, 0.0 ), SvgPoint( 0.0, 1.0 ) );
    grad.stops.emplace_back( 0.0, color_from_hex( "#000000" ) );
    grad.stops.emplace_back( 0.5, color_from_hex( "#c040be" ) );
    grad.stops.emplace_back( 1.0, color_from_hex( "#81bfff" ) );
    doc.defs.push_back( grad );

    auto rect = SvgRect( 0, 0, 10, 100 );
    rect.stroke.color = Color( 0, 0, 0 );
    rect.fill = SvgFill( "bpb" );
    doc << rect;

    doc.margin = SvgMargin( 10, 10 );

    std::ostringstream out;
    doc.write( out );

    // LOG_DBG << out.str();
    // file_write( out.str(), "/home/lucas/test.svg" );
}

TEST( Svg, ColorBar )
{
    auto doc = SvgDocument();
    doc.overflow = SvgDocument::Overflow::kVisible;
    auto pal = SvgColorBarSettings();

    // Nice palette.
    // pal.palette = ColorPalette( color_list_spectral() );
    auto map = ColorMap( color_list_spectral() );
    auto norm = ColorNormalizationDiverging();

    // Even number of colors.
    // pal.palette = ColorPalette({ {0,0,0}, {1,0,0}, {0,0,1}, {0,0,0} });
    // pal.palette = ColorPalette({ {1,0,0}, {0,0,1} });

    // Uneven number of colors.
    // pal.palette = ColorPalette({ {0,0,0}, {1,0,0}, {0,0,1}, {0,1,0}, {0,0,0} });
    // pal.palette = ColorPalette({ {0,0,0}, {0,1,0}, {0,0,0} });

    norm.min_value(  5.0 );
    norm.mid_value( 15.0 );
    norm.max_value( 20.0 );

    // pal.direction = SvgPalette::Direction::kLeftToRight;
    // pal.diverging_palette = true;

    auto const pal_pair = make_svg_color_bar( pal, map, norm );
    doc.defs.push_back( pal_pair.first );
    doc << pal_pair.second;

    std::ostringstream out;
    doc.write( out );

    // LOG_DBG << out.str();
    // file_write( out.str(), "/home/lucas/test.svg" );
}

TEST( Svg, ColorBarBoundaryNorm )
{
    auto doc = SvgDocument();
    doc.overflow = SvgDocument::Overflow::kVisible;

    // Nice palette.
    auto map = ColorMap( color_list_viridis() );
    auto norm = ColorNormalizationBoundary();
    norm.boundaries({ 3.0, 6.0, 8.0, 10.0 });

    auto pal = SvgColorBarSettings();

    pal.direction = SvgColorBarSettings::Direction::kTopToBottom;
    // pal.width = 200;
    // pal.height = 20;

    auto const pal_pair = make_svg_color_bar( pal, map, norm );
    doc.defs.push_back( pal_pair.first );
    doc << pal_pair.second;

    std::ostringstream out;
    doc.write( out );

    // LOG_DBG << out.str();
    // file_write( out.str(), "/home/lucas/test.svg" );
}
