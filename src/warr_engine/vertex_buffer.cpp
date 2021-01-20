
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_render_vertex::w_render_vertex( const w_vec2& pos, const w_uv& uv, const w_color& color, const float glow )
	: x( pos.x ), y( pos.y ), z( RENDER->rs_z_depth ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	e( glow )
{
	t = -1.0f;
}

w_render_vertex::w_render_vertex( const w_vec3& pos, const w_uv& uv, const w_color& color, const float glow )
	: x( pos.x ), y( pos.y ), z( pos.z + RENDER->rs_z_depth ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	e( glow )
{
	t = -1.0f;
}

// ----------------------------------------------------------------------------

w_vertex_buffer::w_vertex_buffer( w_vertex_array_object* vertex_array_object, int verts_per_element )
	: vertex_array_object( vertex_array_object ), verts_per_element( verts_per_element )
{
	glCreateBuffers( 1, &gl_id );
	bind();

	texture_slots.resize( OPENGL->max_texture_image_units );
	total_texture_slots_used = 0;
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
	for( auto& iter : texture_slots )
	{
		iter = nullptr;
	}

	total_texture_slots_used = 0;

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

int w_vertex_buffer::assign_texture_slot( const a_texture* texture )
{
	// if this texture is already in the slot list, return that index
	for( int x = 0 ; x < total_texture_slots_used ; ++x )
	{
		if( texture_slots[ x ]->src_texture->gl_id == texture->src_texture->gl_id )
		{
			return x;
		}
	}

	// we are out of texture slots, so draw the batch and reset
	if( total_texture_slots_used == OPENGL->max_texture_image_units )
	{
		vertex_array_object->draw_and_reset();
	}

	// add the new texture to the slot list
	texture_slots[ total_texture_slots_used ] = texture;
	total_texture_slots_used++;

	return total_texture_slots_used - 1;
}

void w_vertex_buffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void w_vertex_buffer::bind_texture_units()
{
	for( int x = 0 ; x < total_texture_slots_used ; ++x )
	{
		if( texture_slots[ x ] )
		{
#ifndef _FINALRELEASE
			if( RENDER->single_frame_debugger )
			{
				log( "TU: {} bound to \"{}\"", x, texture_slots[ x ]->src_texture->tag );
			}
#endif

			glBindTextureUnit( x, texture_slots[ x ]->src_texture->gl_id );
		}
	}
}

void w_vertex_buffer::upload()
{
	glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof( w_render_vertex ), vertices.data() );
}
