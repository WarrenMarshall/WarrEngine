
#include "master_pch.h"
#include "master_header.h"

w_batch_vert::w_batch_vert( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive )
    : x( pos.x ), y( pos.y ), z( RENDER->rs_z_depth ),
    u( uv.u ), v( uv.v ),
    r( color.r ), g( color.g ), b( color.b ), a( color.a ),
    e( emissive )
{
}

w_batch_vert::w_batch_vert( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive )
    : x( pos.x ), y( pos.y ), z( pos.z + RENDER->rs_z_depth ),
    u( uv.u ), v( uv.v ),
    r( color.r ), g( color.g ), b( color.b ), a( color.a ),
    e( emissive )
{
    // #batch
    assert( false );    // temp disable this for testing
}

// ----------------------------------------------------------------------------

w_index_buffer::w_index_buffer()
{
	glCreateBuffers( 1, &gl_id );
    bind();
}

w_index_buffer::~w_index_buffer()
{
    unbind();
	glDeleteBuffers( 1, &gl_id );
}

void w_index_buffer::bind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gl_id );
}

void w_index_buffer::unbind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

// ----------------------------------------------------------------------------

w_index_buffer_quads::w_index_buffer_quads()
{
    // create the full set of indices right ahead of time since we know
    // the pattern they will be following.

    std::vector<unsigned short> indices;
	indices.resize( w_render_batch::max_quads_per_batch * 6 );

	// quad indices:
	// 0, 1, 2, 0, 2, 3
	// 4, 5, 6, 4, 6, 7
	// ...

	unsigned short offset = 0;
	for( int q = 0 ; q < w_render_batch::max_quads_per_batch * 6 ; q += 6 )
	{
		indices[ q + 0 ] = offset + 0;
		indices[ q + 1 ] = offset + 1;
		indices[ q + 2 ] = offset + 2;

		indices[ q + 3 ] = offset + 0;
		indices[ q + 4 ] = offset + 2;
		indices[ q + 5 ] = offset + 3;

		offset += 4;
	}

    // send the index data to the card

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( w_render_batch::max_quads_per_batch * 6 ) * sizeof( unsigned short ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

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

    // data buffers

	glCreateBuffers( 1, &VBO_id );
    glBindBuffer( GL_ARRAY_BUFFER, VBO_id );

    // preallocate enough space on the card for a maximally sized batch of vertices
    glBufferData(
        GL_ARRAY_BUFFER,
        ( w_render_batch::max_quads_per_batch * 4 ) * sizeof( w_batch_vert ),
        nullptr,
        GL_DYNAMIC_DRAW
    );

	glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_batch_vert ), (const void*) offsetof( w_batch_vert, x ) );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( w_batch_vert ), (const void*) offsetof( w_batch_vert, u ) );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( w_batch_vert ), (const void*) offsetof( w_batch_vert, r ) );
	glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof( w_batch_vert ), (const void*) offsetof( w_batch_vert, e ) );
	glVertexAttribPointer( 4, 1, GL_FLOAT, GL_FALSE, sizeof( w_batch_vert ), (const void*) offsetof( w_batch_vert, t ) );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // index buffer

    switch( render_prim )
    {
        case render_prim::quad:
        {
			index_buffer = std::make_unique<w_index_buffer_quads>();
        }
        break;

        default:
        {
            assert( false );
        }
    }

    // reserve memory

	vertices.reserve( w_render_batch::max_quads_per_batch * 4 );
    texture_slots.resize( OPENGL->max_texture_image_units );

    reset();
}

w_render_batch::~w_render_batch()
{
    unbind();

    glDeleteBuffers( 1, &VBO_id );
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

void w_render_batch::add_quad( const a_texture* tex, const w_batch_vert& v0, const w_batch_vert& v1, const w_batch_vert& v2, const w_batch_vert& v3 )
{
    add_vert( tex, v0 );
    add_vert( tex, v1 );
    add_vert( tex, v2 );
    add_vert( tex, v3 );

    // batch is full, so draw the batch and reset
    if( vertices.size() >= w_render_batch::max_quads_per_batch )
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
    glBindBuffer( GL_ARRAY_BUFFER, VBO_id );
    index_buffer->bind();
}

void w_render_batch::unbind()
{
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
	index_buffer->unbind();
}

// sends the batch to the GPU for drawing and resets to a fresh state

void w_render_batch::draw_and_reset()
{
    auto vertex_count = static_cast<int>( vertices.size() );

    if( vertex_count )
    {
        bind();

        auto index_count = static_cast<int>( vertex_count * 1.5f );

        // bind the textures to the texture units.

        for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
		{
            glBindTextureUnit( x, texture_slots[ x ] );
        }

        // upload just the vertex data we need for this draw call
        glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_count * sizeof( w_batch_vert ), vertices.data() );

        RENDER->stats.draw_calls.inc();
        RENDER->stats.vertices.accum( static_cast<float>( vertex_count ) );
        RENDER->stats.indices.accum( static_cast<float>( index_count ) );

        // draw!

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDrawElements( GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, nullptr );

        unbind();
    }

	// clear out for the next batch
	reset();
}

// clears the render batch to an empty/fresh state

void w_render_batch::reset()
{
    vertices.clear();

    for( auto& iter : texture_slots )
    {
        iter = 0;
    }

    current_texture_slot_idx = -1;
}

void w_render_batch::add_vert( const a_texture* tex, const w_batch_vert& render_vert )
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

    w_batch_vert rv = render_vert;

    // update the position to the transformed position
    rv.x = vtx.x;
	rv.y = vtx.y;

    // find a texture slot for the requested texture
    rv.t = static_cast<float>( assign_texture_slot( tex ) );

    // add the render_vert to the vertex list
    vertices.emplace_back( std::move( rv ) );
}
