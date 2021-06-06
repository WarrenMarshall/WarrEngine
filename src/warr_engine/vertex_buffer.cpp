
#include "master_pch.h"
#include "master_header.h"

namespace war
{

vertex_buffer::vertex_buffer( vertex_array_object* vao, int verts_per_element )
	: vao( vao ), verts_per_element( verts_per_element )
{
	glCreateBuffers( 1, &gl_id );
	bind();

	texture_slots.resize( g_engine->render_api.max_texture_image_units );
	total_texture_slots_used = 0;
	g_engine->render_api.allocate_vertex_buffer_on_gpu( render_batch::max_elements_per_draw_call * verts_per_element, false );
	set_up_vertex_attribs();
	vertices.init_to_size( (size_t)( render_batch::max_elements_per_draw_call * verts_per_element ) );

	unbind();
}

vertex_buffer::~vertex_buffer()
{
	unbind();
	glDeleteBuffers( 1, &gl_id );
}

void vertex_buffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, gl_id );
}

void vertex_buffer::reset()
{
	for( auto& iter : texture_slots )
	{
		iter = nullptr;
	}

	total_texture_slots_used = 0;

	vertices.reset();
}

void vertex_buffer::set_up_vertex_attribs()
{
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );
	glEnableVertexAttribArray( 5 );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( render_vertex ), (const void*)offsetof( render_vertex, x ) );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( render_vertex ), (const void*)offsetof( render_vertex, u ) );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( render_vertex ), (const void*)offsetof( render_vertex, r ) );
	glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof( render_vertex ), (const void*)offsetof( render_vertex, glow ) );
	glVertexAttribIPointer( 4, 1, GL_INT, sizeof( render_vertex ), (const void*)offsetof( render_vertex, texture_id ) );
	glVertexAttribIPointer( 5, 1, GL_INT, sizeof( render_vertex ), (const void*)offsetof( render_vertex, pick_id ) );
}

int vertex_buffer::assign_texture_slot( const texture_asset* texture )
{
	// if this texture is already in the slot list, return that index

	for( int x = 0 ; x < total_texture_slots_used ; ++x )
	{
		if( texture_slots[ x ]->get_src_texture()->gl_id == texture->get_src_texture()->gl_id )
		{
			return x;
		}
	}

	// check if all texture slots are currently in use. if so, flush and reset
	// the vertex array.

	if( total_texture_slots_used == g_engine->render_api.max_texture_image_units )
	{
		vao->flush_and_reset();
	}

	// add the new texture to the slot list

	texture_slots[ total_texture_slots_used ] = texture;
	total_texture_slots_used++;

	return total_texture_slots_used - 1;
}

void vertex_buffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void vertex_buffer::bind_texture_units()
{
	for( int x = 0 ; x < total_texture_slots_used ; ++x )
	{
		if( texture_slots[ x ] )
		{
		#ifndef _FINAL_RELEASE
			if( g_engine->renderer.debug.is_single_frame_logging() )
			{
				log_verbose( "TU: {} bound to \"{}\"", x, texture_slots[ x ]->get_src_texture()->tag );
			}
		#endif

			glBindTextureUnit( x, texture_slots[ x ]->get_src_texture()->gl_id );
		}
	}
}

void vertex_buffer::upload_vertices_to_gpu()
{
	glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.num_objects_in_pool * sizeof( render_vertex ), vertices.data() );
}

}
