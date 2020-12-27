#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_vertex_array_object::w_vertex_array_object( e_render_prim render_prim )
	: render_prim( render_prim )
{
	glCreateVertexArrays( 1, &gl_id );
	glBindVertexArray( gl_id );

	switch( render_prim )
	{
		case render_prim::quad:
		{
			// this computes to 10,000 x 4 vertices, which is 40,000 vertices max,
			// which is 60,000 indices
			//
			// (4 verts / 6 indices = 1 quad)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.5f;
			gl_prim_type = GL_TRIANGLES;

			vertex_buffer = std::make_unique<w_vertex_buffer>( this, 4 );
			index_buffer = std::make_unique<w_index_buffer_quads>( this );
		}
		break;

		case render_prim::triangle:
		{
			// this computes to 10,000 x 3 vertices, which is 30,000 vertices max,
			// which is 60,000 indices
			//
			// (3 verts / 3 indices = 1 triangle)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.0f;
			gl_prim_type = GL_TRIANGLES;

			vertex_buffer = std::make_unique<w_vertex_buffer>( this, 3 );
			index_buffer = std::make_unique<w_index_buffer_tris>( this );
		}
		break;

		case render_prim::line:
		{
			// this computes to 10,000 x 2 vertices, which is 20,000 vertices max,
			// which is 40,000 indices
			//
			// (2 verts / 2 indices = 1 line)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.0f;
			gl_prim_type = GL_LINES;

			vertex_buffer = std::make_unique<w_vertex_buffer>( this, 2 );
			index_buffer = std::make_unique<w_index_buffer_lines>( this );
		}
		break;

		case render_prim::point:
		{
			// this computes to 10,000 x 1 vertices, which is 10,000 vertices max,
			// which is 10,000 indices
			//
			// (1 verts / 1 indices = 1 point)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.0f;
			gl_prim_type = GL_POINTS;

			vertex_buffer = std::make_unique<w_vertex_buffer>( this, 1 );
			index_buffer = std::make_unique<w_index_buffer_lines>( this );
		}
		break;

		default:
		{
			assert( false );
		}
	}
}

w_vertex_array_object::~w_vertex_array_object()
{
	glDeleteVertexArrays( 1, &gl_id );
}

void w_vertex_array_object::bind()
{
	glBindVertexArray( gl_id );
	vertex_buffer->bind();
	index_buffer->bind();
}

void w_vertex_array_object::unbind()
{
	glBindVertexArray( 0 );
	vertex_buffer->unbind();
	index_buffer->unbind();
}
