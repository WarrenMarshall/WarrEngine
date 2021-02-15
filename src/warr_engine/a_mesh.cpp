
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_mesh )

bool a_mesh::create_internals()
{
	assert( !original_filename.empty() );

	auto file = engine->fs->load_text_file( original_filename );

	mesh_verts.clear();

	std::vector<w_vec3> vertex_list;
	std::vector<w_uv> uv_list;

	for( auto& line : ( *file->lines ) )
	{
		if( line.substr( 0, 2 ) == "v " )
		{
			w_tokenizer tok( line, ' ' );

			w_vec3 v;

			tok.get_next_token();	// eat "v "
			v.x = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.y = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.z = w_parser::float_from_str( *( tok.get_next_token() ) );

			vertex_list.emplace_back( std::move( v ) );
		}
		else if( line.substr( 0, 3 ) == "vt " )
		{
			w_tokenizer tok( line, ' ' );

			w_uv v;

			tok.get_next_token();	// eat "vt "
			v.u = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.v = w_parser::float_from_str( *( tok.get_next_token() ) );

			uv_list.emplace_back( std::move( v ) );
		}
		else if( line.substr( 0, 3 ) == "vn " )
		{
			w_tokenizer tok( line, ' ' );

			w_vec3 v;

			tok.get_next_token();	// eat "vn "
			v.x = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.y = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.z = w_parser::float_from_str( *( tok.get_next_token() ) );

			vertex_list.emplace_back( std::move( v ) );
		}
		else if( line.substr( 0, 2 ) == "f " )
		{
			w_tokenizer tok( line, ' ' );

			tok.get_next_token();	// eat "f "

			// each face is broken down as if it's a triangle fan

			int vidx[ 3 ] = {};
			int uvidx[ 3 ] = {};

			for( auto x = 0 ; x < tok.tokens.size() - 1 ; ++x )
			{
				if( x == 0 )
				{
					w_tokenizer tok2( *( tok.get_next_token() ), '/' );

					// first vertex
					vidx[ 0 ] = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
					uvidx[ 0 ] = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
				}
				else if( x == 1 )
				{
					w_tokenizer tok2( *( tok.get_next_token() ), '/' );

					vidx[ 1 ] = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
					uvidx[ 1 ] = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
				}
				else
				{
					w_tokenizer tok2( *( tok.get_next_token() ), '/' );

					vidx[ 2 ] = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
					uvidx[ 2 ] = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;

					// save this vertex info somewhere

					for( auto mvi = 0 ; mvi < 3 ; ++mvi )
					{
						a_mesh_vertex mv;
						mv.pos = vertex_list[ vidx[ mvi ] ];
						mv.uv = uv_list[ uvidx[ mvi ] ];
						mesh_verts.emplace_back( std::move( mv ) );
					}

					// set up for the next triangle by shifting the third vert
					// into the second verts position
					vidx[ 1 ] = vidx[ 2 ];
					uvidx[ 1 ] = uvidx[ 2 ];

					vidx[ 2 ] = -1;
					uvidx[ 2 ] = -1;
				}
			}
		}
	}

	// make sure we have triplets of vertices so it's all triangles
	assert( ( mesh_verts.size() % 3 ) == 0 );

	return true;
}
