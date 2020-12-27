#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_vertex_array_object::w_vertex_array_object( w_render_batch* batch, e_render_prim render_prim )
	: batch( batch), render_prim( render_prim )
{
	glCreateVertexArrays( 1, &gl_id );
	glBindVertexArray( gl_id );

	texture_slots.resize( OPENGL->max_texture_image_units );

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
}

void w_vertex_array_object::unbind()
{
	glBindVertexArray( 0 );
	vertex_buffer->unbind();
}

int w_vertex_array_object::assign_texture_slot( const a_texture* tex )
{
	// if this texture is already in the slot list, return that index
	for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
	{
		if( texture_slots[ x ] == tex )
		{
			current_texture_slot_idx = x;
			return x;
		}
	}

	// we are out of texture slots, so draw the batch and reset
	if( current_texture_slot_idx == ( OPENGL->max_texture_image_units - 1 ) )
	{
		draw_and_reset();
	}

	// add the new texture to the slot list
	current_texture_slot_idx++;
	texture_slots[ current_texture_slot_idx ] = tex;

	return current_texture_slot_idx;
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
	auto vertex_count = static_cast<int>( vertex_buffer->vertices.size() );

	if( vertex_count )
	{
		bind();

		auto index_count = static_cast<int>( vertex_count * indices_to_verts_factor );

		// bind the textures to the texture units.

		for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
		{
			glBindTextureUnit( x, texture_slots[ x ] ? texture_slots[ x ]->gl_id : 0 );
		}

		// upload verts to the card

		vertex_buffer->upload( vertex_count );

		// draw!

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDrawElements( gl_prim_type, index_count, GL_UNSIGNED_SHORT, nullptr );

		// update stats and clean up

		RENDER->stats.draw_calls.inc();
		RENDER->stats.vertices.accum( static_cast<float>( vertex_count ) );
		RENDER->stats.indices.accum( static_cast<float>( index_count ) );

#ifndef _FINALRELEASE
		// frame debugger
		{
			if( RENDER->enable_frame_debugger )
			{
				log( ">> draw call >> prim_type:{}", gl_prim_type );

				for( int x = 0 ; x <= current_texture_slot_idx ; ++x )
				{
					log( "  texture_{} : {}", x, texture_slots[ x ]->tag );
				}

				log( "  {} vertices, {} indices", f_commas( static_cast<float>( vertex_count ) ), f_commas( static_cast<float>( index_count ) ) );
			}
		}
#endif

		unbind();
	}

	// clear out for the next batch
	reset();
}

void w_vertex_array_object::reset()
{
	vertex_buffer->reset();

	for( auto& iter : texture_slots )
	{
		iter = nullptr;
	}

	current_texture_slot_idx = -1;
}
