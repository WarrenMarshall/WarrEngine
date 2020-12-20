
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_mesh )

a_mesh::~a_mesh()
{
	a_mesh::clean_up_internals();
}

void a_mesh::clean_up_internals()
{

}

bool a_mesh::create_internals()
{
	assert( !original_filename.empty() );

	auto file = engine->fs->load_text_file_into_memory( original_filename );

	render_verts.clear();

	std::vector<w_vec3> vertex_list;
	std::vector<w_uv> uv_list;
	std::vector<w_vec3> normal_list;

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

		if( line.substr( 0, 3 ) == "vt " )
		{
			w_tokenizer tok( line, ' ' );

			w_uv v;

			tok.get_next_token();	// eat "vt "
			v.u = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.v = w_parser::float_from_str( *( tok.get_next_token() ) );

			uv_list.emplace_back( std::move( v ) );
		}

		if( line.substr( 0, 3 ) == "vn " )
		{
			w_tokenizer tok( line, ' ' );

			w_vec3 v;

			tok.get_next_token();	// eat "vn "
			v.x = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.y = w_parser::float_from_str( *( tok.get_next_token() ) );
			v.z = w_parser::float_from_str( *( tok.get_next_token() ) );

			vertex_list.emplace_back( std::move( v ) );
	}

#if 0
		if( line.substr( 0, 7 ) == "usemtl " )
		{
			w_tokenizer tok( line, ' ' );
			tok.get_next_token();	// eat "usemtl "
			current_texture = a_texture::find( *( tok.get_next_token() ) );
		}
#endif

		if( line.substr( 0, 2 ) == "f " )
		{
			w_tokenizer tok( line, ' ' );

			tok.get_next_token();	// eat "f "

			for( auto x = 0 ; x < 3 ; ++x )
			{
				w_tokenizer tok2( *( tok.get_next_token() ), '/' );

				int vidx = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;
				int uvidx = w_parser::int_from_str( *( tok2.get_next_token() ) ) - 1;

				w_render_buffer_vert rbv( vertex_list[ vidx ], uv_list[ uvidx ], w_color::white, 0.0f );
				render_verts.emplace_back( std::move( rbv ) );
			}
		}
	}

	// make sure we have triplets of vertices so it's all triangles
	assert( ( render_verts.size() % 3 ) == 0 );

	return true;
}
