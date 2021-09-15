
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Tile_Map_Asset::Tile::Tile( int32_t idx, int32_t x_idx, int32_t y_idx, e_tile_flags_t flags )
	: idx( idx ), x_idx( x_idx ), y_idx( y_idx ), flags( flags )
{
}

// ----------------------------------------------------------------------------

bool Tile_Map_Asset::create()
{
	auto file = File_System::load_text_file( original_filename );

	Layer* current_layer = nullptr;
	Chunk* current_chunk = nullptr;
	Tiled_Object_Group* current_object_group = nullptr;
	Tiled_Object* current_object = nullptr;
	float_t current_object_rotation = 0.f;
	bool inside_data_block = false;
	bool inside_editor_settings = false;
	bool inside_chunk_block = false;
	size_t chunk_w = Chunk::default_chunk_sz, chunk_h = Chunk::default_chunk_sz;
	Rect chunk_bounds;
	int32_t data_block_y = 0;

	for( const auto& raw_line : file->lines )
	{
		auto line = String_Util::trim( raw_line );

		if( line.starts_with( "<map " ) )
		{
			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();
				auto value = subtok.get_next_token();

				if( value.has_value() )
				{
					std::string value_str = std::string( *value );
					String_Util::replace_char( value_str, '\"', ' ' );
					value_str = String_Util::trim( value_str );

					if( key == "width" )
					{
						width = Text_Parser::int_from_str( value_str );
					}
					else if( key == "height" )
					{
						height = Text_Parser::int_from_str( value_str );
					}
					else if( key == "tilewidth" )
					{
						tile_sz = Text_Parser::int_from_str( value_str );
					}
					else if( key == "infinite" )
					{
						is_infinite = Text_Parser::int_from_str( value_str );
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

			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();
				auto value = subtok.get_next_token();

				if( value.has_value() )
				{
					std::string value_str = std::string( *value );
					String_Util::replace_char( value_str, '\"', ' ' );
					value_str = String_Util::trim( value_str );

					if( key == "x" )
					{
						current_chunk->tilemap_bounds.x = Text_Parser::float_from_str( value_str );
					}
					else if( key == "y" )
					{
						current_chunk->tilemap_bounds.y = Text_Parser::float_from_str( value_str );
					}
					else if( key == "width" )
					{
						current_chunk->tilemap_bounds.w = Text_Parser::float_from_str( value_str );
					}
					else if( key == "height" )
					{
						current_chunk->tilemap_bounds.h = Text_Parser::float_from_str( value_str );
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
			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();
				auto value = subtok.get_next_token();

				if( value.has_value() )
				{
					std::string value_str = std::string( *value );
					String_Util::replace_char( value_str, '\"', ' ' );
					value_str = String_Util::trim( value_str );

					if( key == "width" )
					{
						chunk_w = Text_Parser::int_from_str( value_str );
					}
					else if( key == "height" )
					{
						chunk_h = Text_Parser::int_from_str( value_str );
					}
				}
			}
		}
		else if( inside_data_block )
		{
			Tokenizer tok( line, "," );

			for( auto x = 0 ; x < current_chunk->tilemap_bounds.w ; ++x )
			{
				uint32_t idx = String_Util::to_uint( std::string( *tok.get_next_token() ) );

				e_tile_flags_t flags = 0;

				flags |= ( idx & FLIPPED_HORIZONTALLY_FLAG ) > 0 ? e_tile_flags::flipped_horizontally : 0;
				flags |= ( idx & FLIPPED_VERTICALLY_FLAG ) > 0 ? e_tile_flags::flipped_vertically : 0;
				flags |= ( idx & FLIPPED_DIAGONALLY_FLAG ) > 0 ? e_tile_flags::flipped_diagonally : 0;

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

				current_chunk->tilemap_bounds.w = (float)width;
				current_chunk->tilemap_bounds.h = (float)height;

				current_chunk->tiles.reserve( (size_t)( current_chunk->tilemap_bounds.w* current_chunk->tilemap_bounds.h ) );
			}

			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "name" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_layer->tag = *value;
					String_Util::erase_char( current_layer->tag, '\"' );
					String_Util::erase_char( current_layer->tag, '/' );
					String_Util::erase_char( current_layer->tag, '>' );
				}
				else if( *key == "visible" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_layer->is_visible = Text_Parser::bool_from_str( *value );
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

			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "name" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					String_Util::erase_char( value, '\"' );
					String_Util::erase_char( value, '/' );
					String_Util::erase_char( value, '>' );
					current_object_group->tag = value;
				}
				else if( *key == "visible" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_object_group->is_visible = Text_Parser::bool_from_str( *value );
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

			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=", true );

				auto key = subtok.get_next_token();

				if( *key == "x" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					String_Util::erase_char( value, '\"' );
					current_object->rc.x = Text_Parser::float_from_str( value );
				}
				else if( *key == "y" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					String_Util::erase_char( value, '\"' );
					current_object->rc.y = Text_Parser::float_from_str( value );
				}
				else if( *key == "width" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					String_Util::erase_char( value, '\"' );
					current_object->rc.w = Text_Parser::float_from_str( value );
				}
				else if( *key == "height" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					String_Util::erase_char( value, '\"' );
					current_object->rc.h = Text_Parser::float_from_str( value );
				}
				else if( *key == "rotation" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					String_Util::erase_char( value, '\"' );
					current_object_rotation = Text_Parser::float_from_str( value );

					if( !fequals( current_object_rotation, 0.f) )
					{
						// if a shape has rotation, it needs to be turned into a
						// polygonal shape for collision to work properly. AABBs
						// can't be rotated, obviously.

						current_object->collision_type = e_sc_prim_type::polygon;

						auto w = current_object->rc.w;
						auto h = current_object->rc.h;

						current_object->vertices.emplace_back( Vec2( 0.f, 0.f ) );
						current_object->vertices.emplace_back( Vec2( w, 0.f ) );
						current_object->vertices.emplace_back( Vec2( w, h ) );
						current_object->vertices.emplace_back( Vec2( 0.f, h ) );

						current_object->rotate( current_object_rotation );
					}
				}
				else if( *key == "type" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_object->type = *value;
					String_Util::erase_char( current_object->type, '\"' );
				}
				else if( *key == "name" )
				{
					auto value = subtok.get_next_token();
					assert( value.has_value() );

					current_object->name = *value;
					String_Util::erase_char( current_object->name, '\"' );
				}
			}
		}
		else if( line.starts_with( "<polygon " ) )
		{
			assert( current_object_group );
			assert( current_object );

			current_object->collision_type = e_sc_prim_type::polygon;
			current_object->vertices.clear();

			Tokenizer tok( line, " ", true );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

				auto key = subtok.get_next_token();

				if( *key == "points" )
				{
					std::string value = std::string( *subtok.get_next_token() );
					value = value.substr( 1, value.size() - 4 );

					Tokenizer vert_tok( value, " " );

					while( !vert_tok.is_eos() )
					{
						Tokenizer xy_tok( *vert_tok.get_next_token(), "," );

						auto x = String_Util::to_float( std::string( *xy_tok.get_next_token() ) );
						auto y = String_Util::to_float( std::string( *xy_tok.get_next_token() ) );

						current_object->vertices.emplace_back( Vec2( x, y ) );
					}

				#ifdef _DEBUG
					if( current_object->vertices.size() > C2_MAX_POLYGON_VERTS )
					{
						// there is a polygon collision objects in the tile map
						// with more vertices than the simple collision system
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

			current_object->collision_type = e_sc_prim_type::circle;
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
