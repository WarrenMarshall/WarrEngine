
#include "master_pch.h"
#include "master_header.h"

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

