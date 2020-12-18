
#include "master_pch.h"
#include "master_header.h"

w_render_buffer_vert::w_render_buffer_vert( const w_vec2& pos, const w_uv& uv, const w_color& color )
    :   x( pos.x ), y( pos.y ), z( RENDER->rs_z_depth ),
        u( uv.u ), v( uv.v ),
        r( color.r ), g( color.g ), b( color.b ), a( color.a )
{
}

w_render_buffer_vert::w_render_buffer_vert( const w_vec3& pos, const w_uv& uv, const w_color& color )
	: x( pos.x ), y( pos.y ), z( pos.z + RENDER->rs_z_depth ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a )
{
}

// ----------------------------------------------------------------------------

w_render_buffer::w_render_buffer()
{
    glCreateVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    // data buffers

	glCreateBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_render_buffer_vert ), (void*) nullptr );
    glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( w_render_buffer_vert ), (void*) ( sizeof( float ) * 3 ) );
    glEnableVertexAttribArray( 1 );

	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( w_render_buffer_vert ), (void*) ( sizeof( float ) * 5 ) );
    glEnableVertexAttribArray( 2 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // index buffer

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glCreateBuffers( 1, &EBO );

    unbind();
}

w_render_buffer::~w_render_buffer()
{
    unbind();

    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );
    glDeleteVertexArrays( 1, &VAO );
}

void w_render_buffer::add_quad( const w_render_buffer_vert& v0, const w_render_buffer_vert& v1, const w_render_buffer_vert& v2, const w_render_buffer_vert& v3 )
{
    // this looks a little messy but since we know that vertices 0 and 2 are going to be shared
    // in the quad, we can skip the process of transforming them and searching the vertex
    // array by caching the index they are assigned the first time through and re-using it.

    const int idx0 = add_render_vert( v0 );
    add_render_vert( v1 );
    const int idx2 = add_render_vert( v2 );

    indices.emplace_back( idx0 );
    indices.emplace_back( idx2 );
    add_render_vert( v3 );
}

void w_render_buffer::add_triangle( const w_render_buffer_vert& v0, const w_render_buffer_vert& v1, const w_render_buffer_vert& v2 )
{
	add_render_vert( v0 );
	add_render_vert( v1 );
	add_render_vert( v2 );
}

void w_render_buffer::add_line( const w_render_buffer_vert& v0, const w_render_buffer_vert& v1 )
{
    add_render_vert( v0 );
    add_render_vert( v1 );
}

void w_render_buffer::bind()
{
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
}

void w_render_buffer::unbind()
{
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void w_render_buffer::draw( a_texture* tex )
{
    if( !indices.empty() )
    {
   		tex->bind();

        // send the data to the video card
        glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( w_render_buffer_vert ), vertices.data(), GL_DYNAMIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), indices.data(), GL_DYNAMIC_DRAW );

        switch( tex->gl_prim_type )
        {
            case GL_TRIANGLES:
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            }
            break;

            case GL_LINES:
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                engine->white_wire->bind();
            }
            break;

            default:
            {
                log_error( "Unsupported primitive type" );
            }
            break;
        }

        RENDER->stats.render_buffers.inc();
        RENDER->stats.render_vertices.accum( static_cast<float>( vertices.size() ) );
        RENDER->stats.render_indices.accum( static_cast<float>( indices.size() ) );

        glDrawElements( tex->gl_prim_type, static_cast<int>( indices.size() ), GL_UNSIGNED_INT, nullptr );
    }
}

void w_render_buffer::clear()
{
    // retain capacity so we don't have to reallocate the vectors constantly
    vertices.clear();
    indices.clear();
}

int w_render_buffer::add_render_vert( const w_render_buffer_vert& render_vert )
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

    const w_render_buffer_vert rv(
        w_vec2( vtx.x, vtx.y ),
        w_uv( render_vert.u, render_vert.v ),
        w_color( render_vert.r, render_vert.g, render_vert.b, render_vert.a )
    );

    // add the render_vert to the vertex and index lists.

    vertices.emplace_back( rv );
    int idx = static_cast<int>( vertices.size() ) - 1;
    indices.emplace_back( idx );

    return idx;
}
