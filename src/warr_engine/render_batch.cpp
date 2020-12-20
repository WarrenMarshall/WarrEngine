
#include "master_pch.h"
#include "master_header.h"

w_render_batch_vert::w_render_batch_vert( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive )
    :   x( pos.x ), y( pos.y ), z( RENDER->rs_z_depth ),
        u( uv.u ), v( uv.v ),
	    r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	    e( emissive )
{
}

w_render_batch_vert::w_render_batch_vert( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive )
	: x( pos.x ), y( pos.y ), z( pos.z + RENDER->rs_z_depth ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
    e( emissive )
{
    assert( false );    // temp disable this for testing
}

// ----------------------------------------------------------------------------

w_render_batch::w_render_batch()
{
    glCreateVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    // data buffers

	glCreateBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
    glEnableVertexAttribArray( 3 );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (void*) nullptr );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * 3 ) );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * 5 ) );
	glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof( w_render_batch_vert ), (char*) ( sizeof( float ) * 9 ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // index buffer

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glCreateBuffers( 1, &EBO );

    // preload the indices with values since they don't change frame to frame

    //indices.resize( )

    unbind();
}

w_render_batch::~w_render_batch()
{
    unbind();

    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );
    glDeleteVertexArrays( 1, &VAO );
}

void w_render_batch::add_quad( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2, const w_render_batch_vert& v3 )
{
    // this looks a little messy but since we know that vertices 0 and 2 are going to be shared
    // in the quad, we can skip the process of transforming them and searching the vertex
    // array by caching the index they are assigned the first time through and re-using it.

    const unsigned short idx0 = add_render_vert( v0 );
    add_render_vert( v1 );
    const unsigned short idx2 = add_render_vert( v2 );

    indices.emplace_back( idx0 );
    indices.emplace_back( idx2 );
    add_render_vert( v3 );
}

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

void w_render_batch::draw( a_texture* tex )
{
    if( !indices.empty() )
    {
   		tex->bind();

        // send the data to the video card
        glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( w_render_batch_vert ), vertices.data(), GL_DYNAMIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned short ), indices.data(), GL_DYNAMIC_DRAW );

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

		glDrawElements( tex->gl_prim_type, static_cast<int>( indices.size() ), GL_UNSIGNED_SHORT, nullptr );
    }
}

void w_render_batch::clear()
{
    // retain capacity so we don't have to reallocate the vectors constantly
    vertices.clear();
    indices.clear();
}

unsigned short w_render_batch::add_render_vert( const w_render_batch_vert& render_vert )
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

    const w_render_batch_vert rv(
        w_vec2( vtx.x, vtx.y ),
        w_uv( render_vert.u, render_vert.v ),
        w_color( render_vert.r, render_vert.g, render_vert.b, render_vert.a ),
        render_vert.e
    );

    // add the render_vert to the vertex and index lists.

    vertices.emplace_back( rv );
    unsigned short idx = static_cast<unsigned short>( vertices.size() ) - 1;
    indices.emplace_back( idx );

    assert( idx < 65500 );      // getting close to the max value of an unsigned short (65535)

    return idx;
}
