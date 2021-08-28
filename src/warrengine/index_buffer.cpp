
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Index_Buffer::Index_Buffer()
{
	glCreateBuffers( 1, &gl_id );
	bind();
}

Index_Buffer::~Index_Buffer()
{
	unbind();
	glDeleteBuffers( 1, &gl_id );
}

void Index_Buffer::bind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gl_id );
}

void Index_Buffer::unbind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

// ----------------------------------------------------------------------------

Index_Buffer_Quads::Index_Buffer_Quads()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( Primitive_Batch::max_elements_per_draw_call * 6 );

	// index format:
	// 0, 1, 2, 0, 2, 3
	// 4, 5, 6, 4, 6, 7
	// ...

	uint32 offset = 0;
	for( uint32 q = 0 ; q < Primitive_Batch::max_elements_per_draw_call * 6 ; q += 6 )
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
		( Primitive_Batch::max_elements_per_draw_call * 6 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

Index_Buffer_Tris::Index_Buffer_Tris()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( Primitive_Batch::max_elements_per_draw_call * 3 );

	// index format:
	// 0, 1, 2,
	// 3, 4, 5,
	// ...

	for( uint32 q = 0 ; q < Primitive_Batch::max_elements_per_draw_call * 3 ; q++ )
	{
		indices[ q ] = q;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( Primitive_Batch::max_elements_per_draw_call * 3 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

Index_Buffer_Lines::Index_Buffer_Lines()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( Primitive_Batch::max_elements_per_draw_call * 2 );

	// index format:
	// 0, 1,
	// 2, 3,
	// ...

	for( uint32 q = 0 ; q < Primitive_Batch::max_elements_per_draw_call * 2 ; q++ )
	{
		indices[ q ] = q;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( Primitive_Batch::max_elements_per_draw_call * 2 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// ----------------------------------------------------------------------------

Index_Buffer_Points::Index_Buffer_Points()
{
	// create the full set of indices right ahead of time since we know
	// the pattern they will be following.

	std::vector<uint32> indices;
	indices.resize( Primitive_Batch::max_elements_per_draw_call * 1 );

	// index format:
	// 0,
	// 1,
	// ...

	for( uint32 q = 0 ; q < Primitive_Batch::max_elements_per_draw_call * 1 ; q++ )
	{
		indices[ q ] = q;
	}

	// upload the indices to the GPU and discard the source data when the
	// function exits.

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		( Primitive_Batch::max_elements_per_draw_call * 1 ) * sizeof( uint32 ),
		indices.data(),
		GL_STATIC_DRAW
	);
}

}
