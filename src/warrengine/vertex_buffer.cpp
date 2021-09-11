
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Vertex_Buffer::Vertex_Buffer( Vertex_Array_Object* vao, int32_t verts_per_element )
	: vao( vao ), verts_per_element( verts_per_element )
{
	glCreateBuffers( 1, &gl_id );
	bind();

	g_engine->opengl_mgr.allocate_vertex_buffer_on_gpu( Primitive_Batch::max_elements_per_draw_call * verts_per_element, false );
	set_up_vertex_attribs();
	vertices.init_to_size( (size_t)( Primitive_Batch::max_elements_per_draw_call * verts_per_element ) );
	reset();

	unbind();
}

Vertex_Buffer::~Vertex_Buffer()
{
	unbind();
	glDeleteBuffers( 1, &gl_id );
}

void Vertex_Buffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, gl_id );
}

void Vertex_Buffer::reset()
{
	texture_slots.clear();
	texture_slots.resize( g_engine->opengl_mgr.max_texture_image_units );

	total_texture_slots_used = 0;

	cached_texture_slot.clear();

	vertices.reset();
}

void Vertex_Buffer::set_up_vertex_attribs()
{
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );
	glEnableVertexAttribArray( 5 );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Render_Vertex ), (const void*)offsetof( Render_Vertex, x ) );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Render_Vertex ), (const void*)offsetof( Render_Vertex, u ) );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( Render_Vertex ), (const void*)offsetof( Render_Vertex, r ) );
	glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof( Render_Vertex ), (const void*)offsetof( Render_Vertex, glow ) );
	glVertexAttribIPointer( 4, 1, GL_INT, sizeof( Render_Vertex ), (const void*)offsetof( Render_Vertex, texture_id ) );
	glVertexAttribIPointer( 5, 1, GL_INT, sizeof( Render_Vertex ), (const void*)offsetof( Render_Vertex, pick_id ) );
}

size_t Vertex_Buffer::assign_texture_slot( const Texture_Asset* texture )
{
	// if this is the same texture as the last time this function was called,
	// just return that same idx

	if( auto idx = cached_texture_slot.get( texture ) ; idx )
	{
		return *idx;
	}

	// if this texture is already in the slot list, return that index

	auto texture_gl_id = texture->get_src_texture()->gl_id;

	for( size_t x = 0 ; x < total_texture_slots_used ; ++x )
	{
		if( texture_slots[ x ]->get_src_texture()->gl_id == texture_gl_id )
		{
			return x;
		}
	}

	// if we get here, this is a previously unseen texture.
	//
	// if all texture slots are currently in use, flush and reset the vertex
	// array.

	if( total_texture_slots_used == g_engine->opengl_mgr.max_texture_image_units )
	{
		vao->flush_and_reset( e_draw_call::opaque );
		vao->flush_and_reset( e_draw_call::transparent );

		reset();
	}

	// add the new texture to the slot list

	texture_slots[ total_texture_slots_used ] = texture;
	total_texture_slots_used++;

	assert( texture );
	cached_texture_slot.set( texture, total_texture_slots_used - 1 );

	return total_texture_slots_used - 1;
}

void Vertex_Buffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Vertex_Buffer::bind_texture_units()
{
	for( auto x = 0 ; x < total_texture_slots_used ; ++x )
	{
		if( texture_slots[ x ] )
		{
			glBindTextureUnit( x, texture_slots[ x ]->get_src_texture()->gl_id );
		}
	}
}

void Vertex_Buffer::upload_vertices_to_gpu()
{
	// the number of objects in the pool needs to be evenly divisible by the
	// primitive types being stored in this vertex buffer. if not, something is
	// off in the batching code.
	assert( ( vertices.num_objects_in_pool() % verts_per_element ) == 0 );

	glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.num_objects_in_pool() * sizeof( Render_Vertex ), vertices.data() );
}

}
