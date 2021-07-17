
#include "master_pch.h"
#include "master_header.h"

namespace war
{

index_buffer::index_buffer()
{
	glCreateBuffers( 1, &gl_id );
	bind();
}

index_buffer::~index_buffer()
{
	unbind();
	glDeleteBuffers( 1, &gl_id );
}

void index_buffer::bind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gl_id );
}

void index_buffer::unbind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

// ----------------------------------------------------------------------------

index_buffer_quads::index_buffer_quads()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( primitive_batch::max_elements_per_draw_call * 6 );

	// index format:
	// 0, 1, 2, 0, 2, 3
	// 4, 5, 6, 4, 6, 7
	// ...

	uint32 offset = 0;
	for( uint32 q = 0 ; q < primitive_batch::max_elements_per_draw_call * 6 ; q += 6 )
	{
		indices[ q + 0 ] = offset + 0;
		indices[ q + 1 ] = offset + 1;
		indices[ q + 2 ] = offset + 2;

		indices[ q + 3 ] = offset + 0;
		indices[ q + 4 ] = offset + 2;
		indices[ q + 5 ] = offset + 3;

		offset += 4;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( primitive_batch::max_elements_per_draw_call * 6 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

index_buffer_tris::index_buffer_tris()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( primitive_batch::max_elements_per_draw_call * 3 );

	// index format:
	// 0, 1, 2,
	// 3, 4, 5,
	// ...

	for( uint32 q = 0 ; q < primitive_batch::max_elements_per_draw_call * 3 ; q++ )
	{
		indices[ q ] = q;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( primitive_batch::max_elements_per_draw_call * 3 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

index_buffer_lines::index_buffer_lines()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( primitive_batch::max_elements_per_draw_call * 2 );

	// index format:
	// 0, 1,
	// 2, 3,
	// ...

	for( uint32 q = 0 ; q < primitive_batch::max_elements_per_draw_call * 2 ; q++ )
	{
		indices[ q ] = q;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( primitive_batch::max_elements_per_draw_call * 2 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

index_buffer_points::index_buffer_points()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( primitive_batch::max_elements_per_draw_call * 1 );

	// index format:
	// 0,
	// 1,
	// ...

	for( uint32 q = 0 ; q < primitive_batch::max_elements_per_draw_call * 1 ; q++ )
	{
		indices[ q ] = q;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( primitive_batch::max_elements_per_draw_call * 1 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

}
