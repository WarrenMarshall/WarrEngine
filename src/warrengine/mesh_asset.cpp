
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool mesh_asset::create()
{
	assert( !original_filename.empty() );

	auto file = file_system::load_text_file( original_filename );

	mesh_verts.clear();

	std::vector<vec3> vertex_list;
	std::vector<vec2> uv_list;
	texture_asset* current_texture = nullptr;

	for( auto& line : file->lines )
	{
		if( line.substr( 0, 2 ) == "v " )
		{
			tokenizer tok( line, " " );

			vec3 v;

			tok.get_next_token();	// eat "v "
			v.x = text_parser::float_from_str( *( tok.get_next_token() ) );
			v.y = text_parser::float_from_str( *( tok.get_next_token() ) );
			v.z = text_parser::float_from_str( *( tok.get_next_token() ) );

			vertex_list.emplace_back( std::move( v ) );
		}
		else if( line.substr( 0, 3 ) == "vt " )
		{
			tokenizer tok( line, " " );

			vec2 v;

			tok.get_next_token();	// eat "vt "
			v.u = text_parser::float_from_str( *( tok.get_next_token() ) );
			v.v = text_parser::float_from_str( *( tok.get_next_token() ) );

			uv_list.emplace_back( std::move( v ) );
		}
		else if( line.substr( 0, 7 ) == "usemtl " )
		{
			tokenizer tok( line, " " );

			tok.get_next_token();	// eat "usemtl "
			current_texture = g_engine->find_asset<texture_asset>( *tok.get_next_token() );
		}
		else if( line.substr( 0, 2 ) == "f " )
		{
			tokenizer tok( line, " " );

			tok.get_next_token();	// eat "f "

			// each face is broken down as if it's a triangle fan

			int vidx[ 3 ] = {};
			int uvidx[ 3 ] = {};

			for( auto x = 0 ; x < tok.tokens.size() - 1 ; ++x )
			{
				if( x == 0 )
				{
					tokenizer tok2( *( tok.get_next_token() ), "/" );

					// first vertex
					vidx[ 0 ] = text_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
					uvidx[ 0 ] = text_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
				}
				else if( x == 1 )
				{
					tokenizer tok2( *( tok.get_next_token() ), "/" );

					vidx[ 1 ] = text_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
					uvidx[ 1 ] = text_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
				}
				else
				{
					tokenizer tok2( *( tok.get_next_token() ), "/" );

					vidx[ 2 ] = text_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
					uvidx[ 2 ] = text_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;

					// save this vertex info somewhere

					for( auto mvi = 0 ; mvi < 3 ; ++mvi )
					{
						vertex mv;
						mv.pos = vertex_list[ vidx[ mvi ] ];
						mv.uv = uv_list[ uvidx[ mvi ] ];
						mv.texture = current_texture ? current_texture : g_engine->find_asset<texture_asset>( "engine_white" );

						mesh_verts.emplace_back( std::move( mv ) );
					}

					// set up for the next triangle by shifting the third vert
					// into the second verts position
					vidx[ 1 ] = vidx[ 2 ];
					uvidx[ 1 ] = uvidx[ 2 ];

					//vidx[ 2 ] = -1;
					//uvidx[ 2 ] = -1;
				}
			}
		}
	}

	// make sure we have triplets of vertices so it's all triangles
	assert( ( mesh_verts.size() % 3 ) == 0 );

	return true;
}

}
