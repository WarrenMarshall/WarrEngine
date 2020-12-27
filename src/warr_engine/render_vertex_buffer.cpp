
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_vertex_buffer::w_vertex_buffer( w_vertex_array_object* vertex_array_object, int verts_per_element )
	: vertex_array_object( vertex_array_object ), verts_per_element( verts_per_element )
{
	glCreateBuffers( 1, &gl_id );
	bind();

	preallocate_vertices( max_elements_per_render_batch * verts_per_element );
	set_up_vertex_attribs();
	vertices.reserve( max_elements_per_render_batch * verts_per_element );

	unbind();
}

w_vertex_buffer::~w_vertex_buffer()
{
	unbind();
	glDeleteBuffers( 1, &gl_id );
}

void w_vertex_buffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, gl_id );
}

void w_vertex_buffer::reset()
{
	vertices.clear();
}

void w_vertex_buffer::set_up_vertex_attribs()
{
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( w_render_vertex ), (const void*) offsetof( w_render_vertex, x ) );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( w_render_vertex ), (const void*) offsetof( w_render_vertex, u ) );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( w_render_vertex ), (const void*) offsetof( w_render_vertex, r ) );
	glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof( w_render_vertex ), (const void*) offsetof( w_render_vertex, e ) );
	glVertexAttribPointer( 4, 1, GL_FLOAT, GL_FALSE, sizeof( w_render_vertex ), (const void*) offsetof( w_render_vertex, t ) );
}

// pre-allocate memory on the card for all the verts we will ever use

void w_vertex_buffer::preallocate_vertices( int max_verts )
{
	glBufferData(
		GL_ARRAY_BUFFER,
		max_verts * sizeof( w_render_vertex ),
		nullptr,
		GL_DYNAMIC_DRAW
	);
}

void w_vertex_buffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void w_vertex_buffer::upload( int num_verts_to_upload )
{
	glBufferSubData( GL_ARRAY_BUFFER, 0, num_verts_to_upload * sizeof( w_render_vertex ), vertices.data() );
}
