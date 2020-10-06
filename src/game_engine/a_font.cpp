#include "master_pch.h"
#include "master_header.h"

bool a_font_def::create_internals()
{
	ZeroMemory( char_map, sizeof( w_font_char ) * max_font_chars );

	auto file = engine->fs->load_text_file_into_memory( original_filename );

	float x, y, w, h;

	for( const auto& line : *( file.get()->lines.get() ) )
	{
		if( line.substr( 0, 5 ) == "char " )
		{
			// parse a char definition
			int char_id = w_parser::int_from_str( w_parser::key_from_str( line, "id=" ) );
			w_font_char* fch = &( char_map[ char_id ] );

			x = w_parser::float_from_str( w_parser::key_from_str( line, "x=" ) );
			y = w_parser::float_from_str( w_parser::key_from_str( line, "y=" ) );
			w = w_parser::float_from_str( w_parser::key_from_str( line, "width=" ) );
			h = w_parser::float_from_str( w_parser::key_from_str( line, "height=" ) );

			fch->xoffset = w_parser::float_from_str( w_parser::key_from_str( line, "xoffset=" ) );
			fch->yoffset = w_parser::float_from_str( w_parser::key_from_str( line, "yoffset=" ) );
			fch->xadvance = w_parser::float_from_str( w_parser::key_from_str( line, "xadvance=" ) );

			fch->w = w;
			fch->h = h;

			fch->subtex = std::make_unique<a_subtexture>( texture->name, w_rect( x, y, w, h ) );

			max_height = w_max( max_height, static_cast<int>( fch->h + fch->yoffset ) );
		}
	}

	return true;
}

// ----------------------------------------------------------------------------

/*
	computes how wide and how tall a string is using this font.
*/
w_vec2 a_font::get_string_extents( const std::string_view text )
{
	w_font_char* pxch;
	w_vec2 bounds;

	bool inside_color_code = false;

	for( unsigned char iter : text )
	{
		pxch = &( font_def->char_map[ iter ] );

		if( iter == '{' )
		{
			inside_color_code = true;
		}
		else if( iter == '}' )
		{
			inside_color_code = false;
		}
		else if( !inside_color_code )
		{
			bounds.x += pxch->xadvance;
			bounds.y = w_max( bounds.y, pxch->h + pxch->yoffset );
		}
	}

	return bounds;
}
