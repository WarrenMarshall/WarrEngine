
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool tile_set_asset::create()
{
	auto file = file_system::load_text_file( original_filename );
	texture_asset* texture = nullptr;

	for( const auto& line : file->lines )
	{
		if( line.starts_with( "<tileset " ) )
		{
			tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();
				auto value = subtok.get_next_token();

				if( value.has_value() )
				{
					std::string value_str = std::string( *value );
					string_util::replace_char( value_str, '\"', ' ' );
					value_str = string_util::trim( value_str );

					if( key == "tilecount" )
					{
						tile_count = text_parser::int_from_str( value_str );
					}
					else if( key == "tilewidth" )
					{
						tile_width = text_parser::int_from_str( value_str );
					}
					else if( key == "tileheight" )
					{
						tile_height = text_parser::int_from_str( value_str );
					}
				}
			}

			assert( tile_count );
			assert( tile_width );
			assert( tile_height );
		}
		else if( line.starts_with( "<property " ) )
		{
			tokenizer tok( line, " " );

			std::string name;

			while( !tok.is_eos() )
			{
				tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "name" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					std::string value_str = std::string( *value );
					string_util::erase_char( value_str, '\"' );

					name = value_str;
				}

				if( *key == "value" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					std::string value_str = std::string( *value );
					string_util::erase_char( value_str, '\"' );
					string_util::replace_substring( value_str, "/>", "" );

					if( name == "texture_tag" )
					{
						texture = g_engine->find_asset<texture_asset>( value_str );
					}
				}
			}
		}
	}

	// create the individual tiles from the tile set info

	tiles.reserve( tile_count );

	auto w = texture->get_src_texture()->w / tile_width;
	auto h = texture->get_src_texture()->h / tile_height;

	for( auto y = 0 ; y < h ; ++y )
	{
		for( auto x = 0 ; x < w ; ++x )
		{
			texture_asset tile =
				texture_asset(
					texture->get_src_texture()->tag,
					rect( x * tile_width, y * tile_height, tile_width, tile_height )
				);

			tiles.emplace_back( std::move( tile ) );
		}
	}

	return true;
}

}
