
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool font_def_asset::create()
{
	ZeroMemory( char_map, sizeof( glyph ) * max_font_chars );

	auto file = file_system::load_text_file( original_filename );

	uint8 x, y, w, h;
	uint8 char_id;
	glyph* fch;

	for( const auto& line : file->lines )
	{
		if( line.substr( 0, 5 ) == "char " )
		{
			// parse a char definition
			char_id = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "id=" ) );
			fch = &( char_map[ char_id ] );

			x = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "x=" ) );
			y = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "y=" ) );
			w = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "width=" ) );
			h = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "height=" ) );

			fch->xoffset = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "xoffset=" ) );
			fch->yoffset = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "yoffset=" ) );
			fch->xadvance = (uint8)text_parser::uint_from_str( text_parser::key_from_str( line, "xadvance=" ) );

			fch->w = (uint8)w;
			fch->h = (uint8)h;

			fch->glyph_texture = texture_asset( src_texture->tag, rect( x, y, w, h ) );

			max_height = glm::max( max_height, fch->h );
		}
	}

	return true;
}

// ----------------------------------------------------------------------------

// computes how wide and how tall a string is using this font.

vec2 font_asset::get_string_extents( std::string_view text ) const
{
	font_def_asset::glyph* pxch;
	vec2 bounds;

	for( auto ch : text )
	{
		pxch = &( font_def->char_map[ (int)ch ] );

		bounds.x += pxch->xadvance;
		bounds.y = glm::max( bounds.y, (float)( pxch->h + pxch->yoffset ) );
	}

	return bounds;
}

float font_asset::get_max_height()
{
	return font_def->max_height;
}

}
