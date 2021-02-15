
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_font_def )

bool a_font_def::create_internals()
{
	ZeroMemory( char_map, sizeof( w_font_char ) * max_font_chars );

	auto file = engine->fs->load_text_file( original_filename );

	int x, y, w, h;
	int char_id;
	w_font_char* fch;

	for( const auto& line : *( file->lines.get() ) )
	{
		if( line.substr( 0, 5 ) == "char " )
		{
			// parse a char definition
			char_id = w_parser::int_from_str( w_parser::key_from_str( line, "id=" ) );
			fch = &( char_map[ char_id ] );

			x = w_parser::int_from_str( w_parser::key_from_str( line, "x=" ) );
			y = w_parser::int_from_str( w_parser::key_from_str( line, "y=" ) );
			w = w_parser::int_from_str( w_parser::key_from_str( line, "width=" ) );
			h = w_parser::int_from_str( w_parser::key_from_str( line, "height=" ) );

			fch->xoffset = (int16)w_parser::int_from_str( w_parser::key_from_str( line, "xoffset=" ) );
			fch->yoffset = (int16)w_parser::int_from_str( w_parser::key_from_str( line, "yoffset=" ) );
			fch->xadvance = (int16)w_parser::int_from_str( w_parser::key_from_str( line, "xadvance=" ) );

			fch->w = (int16)w;
			fch->h = (int16)h;

			fch->glyph_texture = std::make_unique<a_texture>( src_texture->tag, w_rect( x, y, w, h ) );

			max_height = glm::max( max_height, fch->h );
		}
	}

	return true;
}

// ----------------------------------------------------------------------------

implement_find_func( a_font )

// computes how wide and how tall a string is using this font.

w_vec2 a_font::get_string_extents( const std::string_view text ) const
{
	w_font_char* pxch;
	w_vec2 bounds;

	for( auto ch : text )
	{
		pxch = &( font_def->char_map[ (int)ch ] );

		bounds.x += pxch->xadvance;
		bounds.y = glm::max( bounds.y, (float)( pxch->h + pxch->yoffset ) );
	}

	return bounds;
}
