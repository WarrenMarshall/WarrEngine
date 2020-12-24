
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

// so, this computes to 10,000 x 4 vertices, which is 40,000 vertices,
// which is 60,000 indices ... which is just shy of the limit of
// an insigned short: 65,535
//
// (4 verts / 6 indices = 1 quad)
//
// if you want to make the batches larger, the index buffer will need to
// use a larger data type.

int w_render_batch::max_quads_per_batch = 10000;

w_render_batch::w_render_batch( e_render_prim render_prim )
    : render_prim( render_prim )
{
    glCreateVertexArrays( 1, &VAO_id );
    glBindVertexArray( VAO_id );

    // index buffer

    switch( render_prim )
    {
        case render_prim::quad:
        {
            vertex_buffer = std::make_unique< w_vertex_buffer_quads>();
            index_buffer = std::make_unique<w_index_buffer_quads>();
        }
        break;

        default:
        {
            assert( false );
        }
    }

    texture_slots.resize( OPENGL->max_texture_image_units );

    reset();
}

w_render_batch::~w_render_batch()
{
    unbind();

    glDeleteVertexArrays( 1, &VAO_id );
}

int w_render_batch::assign_texture_slot( const a_texture* tex )
{
	// if this texture is already in the slot list, return that index
	for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
	{
		if( texture_slots[ x ] == static_cast<int>( tex->gl_id ) )
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
	texture_slots[ current_texture_slot_idx ] = tex->gl_id;

    return current_texture_slot_idx;
}

void w_render_batch::add_quad( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2, const w_render_vertex& v3 )
{
    add_vert( tex, v0 );
    add_vert( tex, v1 );
    add_vert( tex, v2 );
    add_vert( tex, v3 );

    // batch is full, so draw the batch and reset
    if( vertex_buffer->vertices.size() >= w_render_batch::max_quads_per_batch )
    {
        draw_and_reset();
    }
}

#if 0 // #batch
void w_render_batch::add_triangle( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2 )
{
	add_render_vert( v0 );
	add_render_vert( v1 );
	add_render_vert( v2 );
}

void w_render_batch::add_line( const w_render_batch_vert& v0, const w_render_batch_vert& v1 )
{
    add_render_vert( v0 );
    add_render_vert( v1 );
}
#endif

void w_render_batch::bind()
{
    glBindVertexArray( VAO_id );
    vertex_buffer->bind();
    index_buffer->bind();
}

void w_render_batch::unbind()
{
    glBindVertexArray( 0 );
	vertex_buffer->unbind();
	index_buffer->unbind();
}

// sends the batch to the GPU for drawing and resets to a fresh state

void w_render_batch::draw_and_reset()
{
    auto vertex_count = static_cast<int>( vertex_buffer->vertices.size() );

    if( vertex_count )
    {
        bind();

        auto index_count = static_cast<int>( vertex_count * 1.5f );

        // bind the textures to the texture units.

        for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
		{
            glBindTextureUnit( x, texture_slots[ x ] );
        }

        // upload verts to the card

        vertex_buffer->upload( vertex_count );

        // draw!

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDrawElements( GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, nullptr );

		// update stats and clean up

		RENDER->stats.draw_calls.inc();
		RENDER->stats.vertices.accum( static_cast<float>( vertex_count ) );
		RENDER->stats.indices.accum( static_cast<float>( index_count ) );

        unbind();
    }

	// clear out for the next batch
	reset();
}

// clears the render batch to an empty/fresh state

void w_render_batch::reset()
{
    vertex_buffer->reset();

    for( auto& iter : texture_slots )
    {
        iter = 0;
    }

    current_texture_slot_idx = -1;
}

void w_render_batch::add_vert( const a_texture* tex, const w_render_vertex& render_vert )
{
    // multiply the current modelview matrix against the vertex being rendered.
    //
    // until this point, the vertex has been in model coordinate space. this
    // moves it into world space.

    w_vec2 vtx = MATRIX->top()->transform_vec2( w_vec2( render_vert.x, render_vert.y ) );

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
    vertex_buffer->vertices.emplace_back( std::move( rv ) );
}
