
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

int w_render_batch::max_elements_per_batch = 10000;

w_render_batch::w_render_batch( e_render_prim render_prim )
{
	vertex_array_object = std::make_unique<w_vertex_array_object>( render_prim );

    texture_slots.resize( OPENGL->max_texture_image_units );

    reset();
}

w_render_batch::~w_render_batch()
{
    unbind();
}

int w_render_batch::assign_texture_slot( const a_texture* tex )
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

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2, const w_render_vertex& v3 )
{
    add_vert( tex, v0 );
    add_vert( tex, v1 );
    add_vert( tex, v2 );
    add_vert( tex, v3 );

	check_draw_and_reset();
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2 )
{
	add_vert( tex, v0 );
	add_vert( tex, v1 );
	add_vert( tex, v2 );

	check_draw_and_reset();
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1 )
{
    add_vert( tex, v0 );
    add_vert( tex, v1 );

	check_draw_and_reset();
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0 )
{
	add_vert( tex, v0 );

	check_draw_and_reset();
}

void w_render_batch::bind()
{
	vertex_array_object->bind();
}

void w_render_batch::unbind()
{
	vertex_array_object->unbind();
}

// check if the batch is full - if so, draw the batch and reset it

void w_render_batch::check_draw_and_reset()
{
	if( vertex_array_object->vertex_buffer->vertices.size() >= w_render_batch::max_elements_per_batch )
	{
		draw_and_reset();
	}
}

// sends the batch to the GPU for drawing and resets to a fresh state

void w_render_batch::draw_and_reset()
{
    auto vertex_count = static_cast<int>( vertex_array_object->vertex_buffer->vertices.size() );

    if( vertex_count )
    {
        bind();

        auto index_count = static_cast<int>( vertex_count * vertex_array_object->indices_to_verts_factor );

        // bind the textures to the texture units.

		for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
		{
			glBindTextureUnit( x, texture_slots[ x ] ? texture_slots[ x ]->gl_id : 0 );
		}

        // upload verts to the card

		vertex_array_object->vertex_buffer->upload( vertex_count );

        // draw!

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDrawElements( vertex_array_object->gl_prim_type, index_count, GL_UNSIGNED_SHORT, nullptr );

		// update stats and clean up

		RENDER->stats.draw_calls.inc();
		RENDER->stats.vertices.accum( static_cast<float>( vertex_count ) );
		RENDER->stats.indices.accum( static_cast<float>( index_count ) );

#ifndef _FINALRELEASE
		// frame debugger
		{
			if( RENDER->enable_frame_debugger )
			{
				log( ">> draw call >> prim_type:{}", vertex_array_object->gl_prim_type );

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

// clears the render batch to an empty/fresh state

void w_render_batch::reset()
{
	vertex_array_object->vertex_buffer->reset();

    for( auto& iter : texture_slots )
    {
        iter = nullptr;
    }

    current_texture_slot_idx = -1;
}

void w_render_batch::add_vert( const a_texture* tex, const w_render_vertex& render_vert )
{
    // multiply the current modelview matrix against the vertex being rendered.
    //
    // until this point, the vertex has been in model coordinate space. this
    // moves it into world space.

    w_vec2 vtx = OPENGL->top()->transform_vec2( w_vec2( render_vert.x, render_vert.y ) );

    // snap to pixel position

    if( RENDER->rs_snap_to_pixel )
    {
        vtx.x = snap_to_pixel( vtx.x );
	    vtx.y = snap_to_pixel( vtx.y );
	}

    w_render_vertex rv = render_vert;

    // update the position to the transformed position
    rv.x = vtx.x;
	rv.y = vtx.y;

    // find a texture slot for the requested texture
    rv.t = static_cast<float>( assign_texture_slot( tex ) );

    // add the render_vert to the vertex list
	vertex_array_object->vertex_buffer->vertices.emplace_back( std::move( rv ) );
}
