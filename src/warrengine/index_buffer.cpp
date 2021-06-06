
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_index_buffer::w_index_buffer( w_vertex_array_object* vertex_array_object )
	: vertex_array_object( vertex_array_object )
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

w_index_buffer_quads::w_index_buffer_quads( w_vertex_array_object* vertex_array_object )
	: w_index_buffer( vertex_array_object )
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint16> indices;
	indices.resize( max_elements_per_render_batch * 6 );

	// index format:
	// 0, 1, 2, 0, 2, 3
	// 4, 5, 6, 4, 6, 7
	// ...

	uint16 offset = 0;
	for( uint16 q = 0 ; q < max_elements_per_render_batch * 6 ; q += 6 )
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
		( max_elements_per_render_batch * 6 ) * sizeof( uint16 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

w_index_buffer_tris::w_index_buffer_tris( w_vertex_array_object* vertex_array_object )
	: w_index_buffer( vertex_array_object )
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<unsigned short> indices;
	indices.resize( max_elements_per_render_batch * 3 );

	// index format:
	// 0, 1, 2,
	// 3, 4, 5,
	// ...

	for( unsigned short q = 0 ; q < max_elements_per_render_batch * 3 ; q++ )
	{
		indices[ q ] = q;
	}

	// send the index data to the card

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( max_elements_per_render_batch * 3 ) * sizeof( unsigned short ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

w_index_buffer_lines::w_index_buffer_lines( w_vertex_array_object* vertex_array_object )
	: w_index_buffer( vertex_array_object )
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<unsigned short> indices;
	indices.resize( max_elements_per_render_batch * 2 );

	// index format:
	// 0, 1,
	// 2, 3,
	// ...

	for( unsigned short q = 0 ; q < max_elements_per_render_batch * 2 ; q++ )
	{
		indices[ q ] = q;
	}

	// send the index data to the card

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( max_elements_per_render_batch * 2 ) * sizeof( unsigned short ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

w_index_buffer_points::w_index_buffer_points( w_vertex_array_object* vertex_array_object )
	: w_index_buffer( vertex_array_object )
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<unsigned short> indices;
	indices.resize( max_elements_per_render_batch * 1 );

	// index format:
	// 0,
	// 1,
	// ...

	for( unsigned short q = 0 ; q < max_elements_per_render_batch * 1 ; q++ )
	{
		indices[ q ] = q;
	}

	// send the index data to the card

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( max_elements_per_render_batch * 1 ) * sizeof( unsigned short ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

