#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_vertex_array_object::w_vertex_array_object( w_render_batch* batch, e_render_prim render_prim )
	: batch( batch), render_prim( render_prim )
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
			// this computes to 104,000 x 3 vertices, which is 30,000 vertices max,
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

int w_vertex_array_object::assign_texture_slot( const a_texture* texture )
{
	return vertex_buffer->assign_texture_slot( texture );
}

void w_vertex_array_object::check_draw_and_reset()
{
	if( vertex_buffer->vertices.size() >= max_elements_per_render_batch )
	{
		draw_and_reset();
	}
}

void w_vertex_array_object::draw_and_reset()
{
	auto index_count = static_cast<int>( vertex_buffer->vertices.size() * indices_to_verts_factor );

	if( index_count )
	{
		bind();

		// upload verts to the card
		vertex_buffer->upload();

		// bind texture units
		vertex_buffer->bind_texture_units();

		// draw
		glDrawElements( gl_prim_type, index_count, GL_UNSIGNED_SHORT, nullptr );

		// update stats and clean up
		RENDER->stats.draw_calls.inc();

		if( render_prim == render_prim::quad ) { RENDER->stats.quads.accum( vertex_buffer->vertices.size() / 4.0f ); }
		else if( render_prim == render_prim::triangle ) { RENDER->stats.triangles.accum( vertex_buffer->vertices.size() / 3.0f ); }
		else if( render_prim == render_prim::line ) { RENDER->stats.lines.accum( vertex_buffer->vertices.size() / 2.0f ); }
		else if( render_prim == render_prim::point ) { RENDER->stats.points.accum( vertex_buffer->vertices.size() / 1.0f ); }

#ifndef _FINALRELEASE
		// frame debugger
		{
			if( RENDER->single_frame_debugger )
			{
				log( ">> draw call >> prim_type:{}", gl_prim_type );

				for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
				{
					if( vertex_buffer->texture_slots[ x ] )
					{
						log( " slot {} : {}", x, vertex_buffer->texture_slots[ x ]->src_texture->tag );
					}
				}

				log( "  {} vertices, {} indices", f_commas( static_cast<float>( vertex_buffer->vertices.size() ) ), f_commas( static_cast<float>( index_count ) ) );
			}
		}
#endif
		unbind();

		// clear out for the next batch
		reset();
	}
}

void w_vertex_array_object::reset()
{
	vertex_buffer->reset();
}
