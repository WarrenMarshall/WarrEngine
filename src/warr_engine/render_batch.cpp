
#include "master_pch.h"
#include "master_header.h"

w_render_batch_vert::w_render_batch_vert( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive )
    : x( pos.x ), y( pos.y ), z( RENDER->rs_z_depth ),
    u( uv.u ), v( uv.v ),
    r( color.r ), g( color.g ), b( color.b ), a( color.a ),
    e( emissive ),
    t( static_cast<float>( RENDER->batch->current_texture_slot_idx ) )
{
}

w_render_batch_vert::w_render_batch_vert( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive )
    : x( pos.x ), y( pos.y ), z( pos.z + RENDER->rs_z_depth ),
    u( uv.u ), v( uv.v ),
    r( color.r ), g( color.g ), b( color.b ), a( color.a ),
    e( emissive ),
    t( static_cast<float>( RENDER->batch->current_texture_slot_idx ) )
{
    // #batch
    assert( false );    // temp disable this for testing
}

// ----------------------------------------------------------------------------

// so, this computes to 10,000 x 4 vertices, which is 40,000 vertices,
// which is 60,000 indices ... which is just shy of the limit of
// an insigned short: 65,535
//
// if you want to make the batches larger, the index buffer will need to
// use a larger data type.

int w_render_batch::max_quads_per_batch = 10000;
int w_render_batch::max_elements_vertices = max_quads_per_batch * 4;

w_render_batch::w_render_batch()
{
    glCreateVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    // data buffers

	glCreateBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );

    unsigned short offset = 0;

    // position
	glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * offset ) );
    offset += 3;

    // texture coordinates
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * offset ) );
    offset += 2;

    // color + alpha
    glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * offset ) );
    offset += 4;

    // emissive
    glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * offset ) );
    offset += 1;

    // texture_id
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 1, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * offset ) );
    offset += 1;

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // index buffer

    glCreateBuffers( 1, &EBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );

    // preload the indices with values since they don't change frame to frame

	std::vector<unsigned short> quad_indices;
    quad_indices.resize( w_render_batch::max_quads_per_batch * 6 );

    offset = 0;
    for( int q = 0 ; q < w_render_batch::max_quads_per_batch * 6 ; q += 6 )
    {
        quad_indices[ q + 0 ] = offset + 0;
        quad_indices[ q + 1 ] = offset + 1;
        quad_indices[ q + 2 ] = offset + 2;

        quad_indices[ q + 3 ] = offset + 0;
        quad_indices[ q + 4 ] = offset + 2;
        quad_indices[ q + 5 ] = offset + 3;

        offset += 4;
    }

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        ( w_render_batch::max_quads_per_batch * 6 ) * sizeof( unsigned short ),
        quad_indices.data(),
        GL_STATIC_DRAW
    );

    unbind();

    texture_slots.resize( OPENGL->max_texture_image_units );

    reset();
}

w_render_batch::~w_render_batch()
{
    unbind();

    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );
    glDeleteVertexArrays( 1, &VAO );
}

void w_render_batch::set_current_texture( a_texture* tex )
{
	current_texture = tex;

	// if this texture is already in the slot list, return that index
	for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
	{
		if( texture_slots[ x ] == static_cast<int>( tex->gl_id ) )
		{
			current_texture_slot_idx = x;
			return;
		}
	}

	// we are out of texture slots, so draw the batch and reset
	if( current_texture_slot_idx == ( OPENGL->max_texture_image_units - 1 ) )
	{
		draw_and_reset();
	}

	// otherwise, add it to the bind list
	current_texture_slot_idx++;
	texture_slots[ current_texture_slot_idx ] = tex->gl_id;
}

void w_render_batch::add_quad( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2, const w_render_batch_vert& v3 )
{
    add_render_vert( v0 );
    add_render_vert( v1 );
    add_render_vert( v2 );
    add_render_vert( v3 );

    num_quads_to_render++;

    // batch is full, so draw the batch and reset
    if( num_quads_to_render >= w_render_batch::max_quads_per_batch )
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
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
}

void w_render_batch::unbind()
{
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

// sends the batch to the GPU for drawing and resets to a fresh state

void w_render_batch::draw_and_reset()
{
    if( num_quads_to_render )
    {
        // bind the textures to the texture units. any unused texture units will be unbound.

        for( int x = 0 ; x < OPENGL->max_texture_image_units ; ++x )
        {
            glBindTextureUnit( x, texture_slots[ x ] );
        }

        //OPENGL->set_uniform_array( "u_textures", OPENGL->texture_slots.data(), OPENGL->max_texture_image_units );

        // upload the vertex data

        glBufferData(
            GL_ARRAY_BUFFER,
            ( num_quads_to_render * 4 ) * sizeof( w_render_batch_vert ),
            vertices.data(),
            GL_DYNAMIC_DRAW
        );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        RENDER->stats.draw_calls.inc();
        RENDER->stats.vertices.accum( static_cast<float>( num_quads_to_render * 4 ) );
        RENDER->stats.indices.accum( static_cast<float>( num_quads_to_render * 6 ) );

        // draw!

		glDrawElements( GL_TRIANGLES, num_quads_to_render * 6, GL_UNSIGNED_SHORT, nullptr );
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

    current_texture = nullptr;
    current_texture_slot_idx = -1;
    num_quads_to_render = 0;
}

void w_render_batch::add_render_vert( const w_render_batch_vert& render_vert )
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

    w_render_batch_vert rv = render_vert;
    rv.x = vtx.x;
	rv.y = vtx.y;

    // add the render_vert to the vertex list.
    vertices.emplace_back( std::move( rv ) );
}
