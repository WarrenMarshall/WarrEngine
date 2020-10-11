
#include "master_pch.h"
#include "master_header.h"

w_render_vert::w_render_vert( const w_vec2& pos, const w_uv& uv, const w_color& color )
    :   x( pos.x ), y( pos.y ), z( RENDER->zdepth ),
        u( uv.u ), v( uv.v ),
        r( color.r ), g( color.g ), b( color.b ), a( color.a )
{
}

// ----------------------------------------------------------------------------

w_render_buffer::w_render_buffer()
{
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    glGenBuffers( 1, &VBO );

    // data buffers

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_render_vert ), (void*) nullptr );
    glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( w_render_vert ), (void*) ( sizeof( float ) * 3 ) );
    glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( w_render_vert ), (void*) ( sizeof( float ) * 5 ) );
    glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // index buffer

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glGenBuffers( 1, &EBO );

    unbind();
}

w_render_buffer::~w_render_buffer()
{
    unbind();

    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );
    glDeleteVertexArrays( 1, &VAO );
}

void w_render_buffer::add_quad( const w_render_vert& v0, const w_render_vert& v1, const w_render_vert& v2, const w_render_vert& v3 )
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

void w_render_buffer::add_line( const w_render_vert& v0, const w_render_vert& v1 )
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
        glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( w_render_vert ), vertices.data(), GL_DYNAMIC_DRAW );
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
    vertices = {};
    indices = {};
}

void w_render_buffer::log_stats( i_asset* asset )
{
    if( !indices.empty() )
    {
        log_msg( "\t\t[{}]: [{} verts, {} indices]",
                              asset->name,
                              f_commas( static_cast<float>( vertices.size() ) ),
                              f_commas( static_cast<float>( indices.size() ) )
        );
    }
}

int w_render_buffer::add_render_vert( const w_render_vert& render_vert )
{
    // multiply the current modelview matrix against the vertex being rendered.
    //
    // until this point, the vertex has been in model coordinate space. this
    // moves it into world space.

    w_vec3 vtx( render_vert.x, render_vert.y, render_vert.z );
    MATRIX->top()->transform_vec3( vtx );

    const w_render_vert rv(
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
