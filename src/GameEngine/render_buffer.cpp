
#include "master_pch.h"
#include "master_header.h"

w_render_vert::w_render_vert( const w_vec3& pos, const w_uv& uv, const w_color& color )
    :   x( pos.x ), y( pos.y ), z( pos.z ),
        u( uv.u ), v( uv.v ),
        r( color.r ), g( color.g ), b( color.b ), a( color.a )
{
}

// ----------------------------------------------------------------------------

w_render_buffer::w_render_buffer()
{
    // this ctor is invalid
    assert( false );
}

w_render_buffer::w_render_buffer( unsigned int prim_type )
    : prim_type( prim_type )
{
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );

    glGenBuffers( 1, &VBO );

    // data buffers

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_render_vert ), (void*) 0 );
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

int w_render_buffer::add( int render_pass, const w_render_vert& render_vert )
{
    // apply the current modelview matrix against the vertex being rendered.
    // until this point, the vertex has been in model coordinate space.

    glm::vec4 v4( render_vert.x, render_vert.y, render_vert.z, 1.0f );
    v4 = (*engine->opengl->top_matrix()) * v4;

    w_render_vert rv( w_vec3( v4.x, v4.y, v4.z ), w_uv( render_vert.u, render_vert.v ), w_color( render_vert.r, render_vert.g, render_vert.b ) );
    
    // look through the existing list of vertices and see if we can find
    // a matching vertex. if one is found, return the index of that vertex
    // instead of putting the new one into the list.

    int idx = 0;
    for( auto& iter : vertices[render_pass] )
    {
        if( iter.is_same( rv ) )
        {
            indices[render_pass].push_back( idx );
            return idx;
        }

        idx++;
    }

    // we have a unique vertex, so add it to the vertex and index lists.

    vertices[render_pass].push_back( std::move( rv ) );
    indices[render_pass].push_back( (int)vertices[render_pass].size() - 1 );

    // return the newly created index

    return static_cast<int>( indices[render_pass].back() );
}

void w_render_buffer::add_quad( const w_render_vert& v0, const w_render_vert& v1, const w_render_vert& v2, const w_render_vert& v3 )
{
    int render_pass = static_cast<int>( e_render_pass::opaque );
    if( !fequals( v0.a + v1.a + v2.a + v3.a, 4.0f ) )
    {
        render_pass = static_cast<int>( e_render_pass::transparent );
    }

    // this looks a little messy but since we know that vertices 0 and 2 are going to be shared
    // in the quad, we can skip the process of transforming them and searching the vertex
    // array by caching the index they are assigned the first time through and re-using it.

    int idx0 = add( render_pass, v0 );
    add( render_pass, v1 );
    int idx2 = add( render_pass, v2 );

    indices[render_pass].push_back( idx0 );
    indices[render_pass].push_back( idx2 );
    add( render_pass, v3 );
}

void w_render_buffer::add_line( const w_render_vert& v0, const w_render_vert& v1 )
{
    int render_pass = static_cast<int>( e_render_pass::opaque );
    if( !fequals( v0.a + v1.a, 2.0f ) )
    {
        render_pass = static_cast<int>( e_render_pass::transparent );
    }

    add( render_pass, v0 );
    add( render_pass, v1 );
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

void w_render_buffer::draw( int render_pass )
{
    if( indices[render_pass].size() > 0 )
    {
        // bind the buffers
        bind();

        // send the data to the video card
        glBufferData( GL_ARRAY_BUFFER, vertices[render_pass].size() * sizeof( w_render_vert ), vertices[render_pass].data(), GL_DYNAMIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices[render_pass].size() * sizeof( unsigned int ), indices[render_pass].data(), GL_DYNAMIC_DRAW );

        switch( prim_type )
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
                log_error( "%s : unsupported primitive type", __FUNCTION__ );
            }
            break;
        }

        engine->render->stats.render_buffers.inc();
        engine->render->stats.render_vertices.accum( static_cast<float>( vertices[render_pass].size() ) );
        engine->render->stats.render_indices.accum( static_cast<float>( indices[render_pass].size() ) );

    glDrawElements( prim_type, (int) indices[render_pass].size(), GL_UNSIGNED_INT, 0 );

    // clean up
    unbind();
    }
}

void w_render_buffer::clear()
{
    vertices[0].clear();
    indices[0].clear();

    vertices[1].clear();
    indices[1].clear();
}

void w_render_buffer::log_stats( i_asset* asset )
{
    if( indices[0].size() || indices[1].size() )
    {
	    log_msg( "\t\t[%s]: [%d verts, %d indices]", asset->name.c_str(), vertices[0].size() + vertices[1].size(), indices[0].size() + indices[1].size() );
    }
}
