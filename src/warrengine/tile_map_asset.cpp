
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

tile_map_asset::tile::tile( int idx, e_tile_flags flags )
	: idx( idx ), flags( flags )
{
}

// ----------------------------------------------------------------------------

bool tile_map_asset::create()
{
	auto file = file_system::load_text_file( original_filename );

	std::optional<layer> current_layer = std::nullopt;
	bool inside_data_block = false;
	int data_block_y = 0;

	for( const auto& raw_line : file->lines )
	{
		auto line = string_util::trim( raw_line );

		if( line.starts_with( "<map " ) )
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

					if( key == "width" )
					{
						width = text_parser::int_from_str( value_str );
					}
					else if( key == "height" )
					{
						height = text_parser::int_from_str( value_str );
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

			assert( width );
			assert( height );
		}
		else if( line.starts_with( "<data" ) )
		{
			inside_data_block = true;
			data_block_y = 0;
		}
		else if( line.starts_with( "</data>" ) )
		{
			inside_data_block = false;
		}
		else if( inside_data_block )
		{
			tokenizer tok( line, "," );

			for( auto x = 0 ; x < width ; ++x )
			{
				unsigned idx = string_util::to_uint( std::string( *tok.get_next_token() ) );

				e_tile_flags flags = 0;

				flags |= ( idx & FLIPPED_HORIZONTALLY_FLAG ) > 0 ? tile_flags::flipped_horizontally : 0;
				flags |= ( idx & FLIPPED_VERTICALLY_FLAG ) > 0 ? tile_flags::flipped_vertically : 0;
				flags |= ( idx & FLIPPED_DIAGONALLY_FLAG ) > 0 ? tile_flags::flipped_diagonally : 0;

				idx &= ~( FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG );

				current_layer->tiles.emplace_back( idx, flags );
			}

			data_block_y++;
		}
		else if( line.starts_with( "<layer" ) )
		{
			current_layer = layer();
			current_layer->tiles.reserve( size_t( width * height ) );

			tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "name" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_layer->tag = *value;
					string_util::erase_char( current_layer->tag, '\"' );
				}
				else if( *key == "visible" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_layer->is_visible = text_parser::int_from_str( *value );
				}
			}
		}
		else if( line.starts_with( "</layer>" ) )
		{
			layers.push_back( *current_layer );
			current_layer = std::nullopt;
		}
	}

	assert( !current_layer.has_value() );

	return true;
}

}
