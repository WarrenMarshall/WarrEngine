
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool Tile_Set_Asset::create()
{
	// load tile index specific data

	auto file = File_System::load_text_file( original_filename );

	Tile_Def* current_tile_def = nullptr;
	Tiled_Object* current_object = nullptr;
	float current_object_rotation = 0.f;

	for( const auto& raw_line : file->lines )
	{
		auto line = String_Util::trim( raw_line );

		if( line.starts_with( "<tileset " ) )
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

					if( key == "tilewidth" )
					{
						tile_width = Text_Parser::int_from_str( value_str );
					}
					else if( key == "tileheight" )
					{
						tile_height = Text_Parser::int_from_str( value_str );
					}
					else if( key == "tilecount" )
					{
						tile_count = Text_Parser::int_from_str( value_str );
					}
					else if( key == "columns" )
					{
						columns = Text_Parser::int_from_str( value_str );
					}
				}
			}

			assert( tile_width );
			assert( tile_height );
			assert( tile_count );
			assert( columns );

			// now that we have the header from the tileset, we can create the base tile definitions

			tile_definitions.clear();
			tile_definitions.reserve( tile_count );

			auto w = texture->get_src_texture()->w / tile_width;
			auto h = texture->get_src_texture()->h / tile_height;

			for( auto y = 0 ; y < h ; ++y )
			{
				for( auto x = 0 ; x < w ; ++x )
				{
					tile_definitions.emplace_back();
					auto tile_def = &tile_definitions.back();

					tile_def->texture = Texture_Asset( texture->get_src_texture()->tag,
						Rect( x * tile_width, y * tile_height, tile_width, tile_height ) );
				}
			}
		}
		else if( line.starts_with( "<tile " ) )
		{
			int tile_idx = -1;
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

					if( key == "id" )
					{
						tile_idx = Text_Parser::int_from_str( value_str );
					}
				}
			}

			assert( tile_idx > -1 );

			current_tile_def = &tile_definitions[ tile_idx ];
			current_object = nullptr;
		}
		else if( line.starts_with( "</tile>" ) )
		{
			current_tile_def = nullptr;
			current_object = nullptr;
		}
		else if( line.starts_with( "<object " ) )
		{
			assert( current_tile_def );

			current_tile_def->objects.emplace_back();
			current_object = &current_tile_def->objects.back();

			Tokenizer tok( line, " " );

			while( !tok.is_eos() )
			{
				Tokenizer subtok( *tok.get_next_token(), "=" );

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

					if( !fequals( current_object_rotation, 0.f ) )
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
			}
		}
		else if( line.starts_with( "<polygon " ) )
		{
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

			current_object->collision_type = e_sc_prim_type::circle;
			current_object->radius = current_object->rc.w / 2.f;
		}
		else if( line.starts_with( "</objectgroup" ) )
		{
			current_object->rotate( current_object_rotation );
			current_object = nullptr;
			current_object_rotation = 0.f;
		}
	}

	return true;
}

}
