
#include "master_pch.h"

namespace war
{

bool Font_Def_Asset::create()
{
	ZeroMemory( char_map, sizeof( Glyph ) * max_font_chars );

	auto file = File_System::load_text_file( original_filename );

	uint8 x, y, w, h;
	uint8 char_id;
	Glyph* fch;

	for( const auto& line : file->lines )
	{
		if( line.substr( 0, 5 ) == "char " )
		{
			// parse a char definition
			char_id = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "id=" ) );
			fch = &( char_map[ char_id ] );

			x = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "x=" ) );
			y = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "y=" ) );
			w = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "width=" ) );
			h = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "height=" ) );

			fch->xoffset = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "xoffset=" ) );
			fch->yoffset = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "yoffset=" ) );
			fch->xadvance = (uint8)Text_Parser::uint_from_str( Text_Parser::key_from_str( line, "xadvance=" ) );

			fch->w = (uint8)w;
			fch->h = (uint8)h;

			fch->glyph_texture = Texture_Asset( src_texture->tag, Rect( x, y, w, h ) );

			max_height = glm::max( max_height, fch->h );
		}
	}

	return true;
}

// ----------------------------------------------------------------------------

// computes how wide and how tall a string is using this font.

Vec2 Font_Asset::get_string_extents( std::string_view text ) const
{
	Font_Def_Asset::Glyph* pxch;
	Vec2 bounds;

	for( auto ch : text )
	{
		pxch = &( font_def->char_map[ (i32)ch ] );

		bounds.x += pxch->xadvance;
		bounds.y = glm::max( bounds.y, (f32)( pxch->h + pxch->yoffset ) );
	}

	return bounds;
}

f32 Font_Asset::get_max_height()
{
	return font_def->max_height;
}

}
