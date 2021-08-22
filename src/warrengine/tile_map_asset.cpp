
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

tile_map_asset::tile::tile( int idx, int x_idx, int y_idx, e_tile_flags flags )
	: idx( idx ), x_idx( x_idx ), y_idx( y_idx ), flags( flags )
{
}

// ----------------------------------------------------------------------------

bool tile_map_asset::create()
{
	auto file = file_system::load_text_file( original_filename );

	layer* current_layer = nullptr;
	chunk* current_chunk = nullptr;
	object_group* current_object_group = nullptr;
	object* current_object = nullptr;
	float current_object_rotation = 0.f;
	bool inside_data_block = false;
	bool inside_editor_settings = false;
	bool inside_chunk_block = false;
	size_t chunk_w = chunk::default_chunk_sz, chunk_h = chunk::default_chunk_sz;
	rect chunk_bounds;
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
						tile_sz = text_parser::int_from_str( value_str );
					}
					else if( key == "infinite" )
					{
						is_infinite = text_parser::int_from_str( value_str );
					}
				}
			}

			assert( width );
			assert( height );
			assert( tile_sz );
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
		else if( line.starts_with( "<editorsettings>" ) )
		{
			inside_editor_settings = true;
		}
		else if( line.starts_with( "</editorsettings>" ) )
		{
			inside_editor_settings = false;
		}
		else if( line.starts_with( "<chunk" ) )
		{
			assert( current_layer );

			data_block_y = 0;
			inside_chunk_block = true;
			current_layer->chunks.emplace_back();
			current_chunk = &current_layer->chunks.back();

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

					if( key == "x" )
					{
						current_chunk->tilemap_bounds.x = text_parser::float_from_str( value_str );
					}
					else if( key == "y" )
					{
						current_chunk->tilemap_bounds.y = text_parser::float_from_str( value_str );
					}
					else if( key == "width" )
					{
						current_chunk->tilemap_bounds.w = text_parser::float_from_str( value_str );
					}
					else if( key == "height" )
					{
						current_chunk->tilemap_bounds.h = text_parser::float_from_str( value_str );
					}
				}
			}

			current_chunk->tiles.reserve( (size_t)( current_chunk->tilemap_bounds.w * current_chunk->tilemap_bounds.h ) );
		}
		else if( line.starts_with( "</chunk>" ) )
		{
			inside_chunk_block = false;
			current_chunk = nullptr;
		}
		else if( inside_editor_settings and line.starts_with( "<chunksize" ) )
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
						chunk_w = text_parser::int_from_str( value_str );
					}
					else if( key == "height" )
					{
						chunk_h = text_parser::int_from_str( value_str );
					}
				}
			}
		}
		else if( inside_data_block )
		{
			tokenizer tok( line, "," );

			for( auto x = 0 ; x < current_chunk->tilemap_bounds.w ; ++x )
			{
				unsigned idx = string_util::to_uint( std::string( *tok.get_next_token() ) );

				e_tile_flags flags = 0;

				flags |= ( idx & FLIPPED_HORIZONTALLY_FLAG ) > 0 ? tile_flags::flipped_horizontally : 0;
				flags |= ( idx & FLIPPED_VERTICALLY_FLAG ) > 0 ? tile_flags::flipped_vertically : 0;
				flags |= ( idx & FLIPPED_DIAGONALLY_FLAG ) > 0 ? tile_flags::flipped_diagonally : 0;

				idx &= ~( FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG );

				current_chunk->tiles.emplace_back( idx - 1, x, data_block_y, flags );
			}

			data_block_y++;
		}
		else if( line.starts_with( "<layer" ) )
		{
			layers.emplace_back();
			current_layer = &layers.back();

			if( !is_infinite )
			{
				// finite maps have a single chunk inside of each layer,
				// representing the entire map

				current_layer->chunks.emplace_back();
				current_chunk = &current_layer->chunks.back();

				current_chunk->tilemap_bounds.w = width;
				current_chunk->tilemap_bounds.h = height;

				current_chunk->tiles.reserve( (size_t)( current_chunk->tilemap_bounds.w* current_chunk->tilemap_bounds.h ) );
			}

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

					current_layer->is_visible = text_parser::bool_from_str( *value );
				}
			}
		}
		else if( line.starts_with( "</layer>" ) )
		{
			current_layer = nullptr;
		}
		else if( line.starts_with( "<objectgroup" ) )
		{
			object_groups.emplace_back();
			current_object_group = &object_groups.back();

			tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "name" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					string_util::erase_char( value, '\"' );
					string_util::erase_char( value, '>' );
					current_object_group->tag = value;
				}
				else if( *key == "visible" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_object_group->is_visible = text_parser::bool_from_str( *value );
				}
			}
		}
		else if( line.starts_with( "</objectgroup" ) )
		{
			current_object_group = nullptr;
		}
		else if( line.starts_with( "<object " ) )
		{
			assert( current_object_group );
			current_object_group->objects.emplace_back();
			current_object = &current_object_group->objects.back();

			tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "x" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					string_util::erase_char( value, '\"' );
					current_object->rc.x = text_parser::float_from_str( value );
				}
				else if( *key == "y" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					string_util::erase_char( value, '\"' );
					current_object->rc.y = text_parser::float_from_str( value );
				}
				else if( *key == "width" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					string_util::erase_char( value, '\"' );
					current_object->rc.w = text_parser::float_from_str( value );
				}
				else if( *key == "height" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					string_util::erase_char( value, '\"' );
					current_object->rc.h = text_parser::float_from_str( value );
				}
				else if( *key == "rotation" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					string_util::erase_char( value, '\"' );
					current_object_rotation = text_parser::float_from_str( value );

					if( !fequals( current_object_rotation, 0.f) )
					{
						// if a shape has rotation, it needs to be turned into a
						// polygonal shape for collision to work properly. AABBs
						// can't be rotated, obviously.

						current_object->collision_type = sc_prim_type::polygon;

						auto w = current_object->rc.w;
						auto h = current_object->rc.h;

						current_object->vertices.emplace_back( vec2( 0.f, 0.f ) );
						current_object->vertices.emplace_back( vec2( w, 0.f ) );
						current_object->vertices.emplace_back( vec2( w, h ) );
						current_object->vertices.emplace_back( vec2( 0.f, h ) );

						current_object->rotate( current_object_rotation );
					}
				}
				else if( *key == "type" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_object->type = *value;
					string_util::erase_char( current_object->type, '\"' );
				}
			}
		}
		else if( line.starts_with( "<polygon " ) )
		{
			assert( current_object_group );
			assert( current_object );

			current_object->collision_type = sc_prim_type::polygon;
			current_object->vertices.clear();

			tokenizer tok( line, " ", true );

			while( !tok.is_eos() )
			{
				tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "points" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					value = value.substr( 1, value.size() - 4 );

					tokenizer vert_tok( value, " " );

					while( !vert_tok.is_eos() )
					{
						tokenizer xy_tok( *vert_tok.get_next_token(), "," );

						auto x = string_util::to_float( std::string( *xy_tok.get_next_token() ) );
						auto y = string_util::to_float( std::string( *xy_tok.get_next_token() ) );

						current_object->vertices.emplace_back( vec2( x, y ) );
					}

				#ifdef _DEBUG
					if( current_object->vertices.size() > C2_MAX_POLYGON_VERTS )
					{
						// there is a polygon collision objects in the tile map
						// with more vertuces than the simple collision system
						// is configured to handle. either increase
						// C2_MAX_POLYGON_VERTS or edit the collider to be
						// within limits.

						log_warning( "too many vertices in polygon shape : {} (max: {})",
							current_object->vertices.size(), C2_MAX_POLYGON_VERTS );
					}
				#endif
				}
			}
		}
		else if( line.starts_with( "<ellipse" ) )
		{
			assert( current_object );

			current_object->collision_type = sc_prim_type::circle;
			current_object->radius = current_object->rc.w / 2.f;
		}
		else if( line.starts_with( "</object" ) )
		{
			current_object->rotate( current_object_rotation );
			current_object = nullptr;
			current_object_rotation = 0.f;
		}
	}

	return true;
}

}
