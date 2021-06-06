
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool font_def_asset::create()
{
	ZeroMemory( char_map, sizeof( glyph ) * max_font_chars );

	auto file = file_system::load_text_file( original_filename );

	int x, y, w, h;
	int char_id;
	glyph* fch;

	for( const auto& line : file->lines )
	{
		if( line.substr( 0, 5 ) == "char " )
		{
			// parse a char definition
			char_id = text_parser::int_from_str( text_parser::key_from_str( line, "id=" ) );
			fch = &( char_map[ char_id ] );

			x = text_parser::int_from_str( text_parser::key_from_str( line, "x=" ) );
			y = text_parser::int_from_str( text_parser::key_from_str( line, "y=" ) );
			w = text_parser::int_from_str( text_parser::key_from_str( line, "width=" ) );
			h = text_parser::int_from_str( text_parser::key_from_str( line, "height=" ) );

			fch->xoffset = (int16)text_parser::int_from_str( text_parser::key_from_str( line, "xoffset=" ) );
			fch->yoffset = (int16)text_parser::int_from_str( text_parser::key_from_str( line, "yoffset=" ) );
			fch->xadvance = (int16)text_parser::int_from_str( text_parser::key_from_str( line, "xadvance=" ) );

			fch->w = (int16)w;
			fch->h = (int16)h;

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
