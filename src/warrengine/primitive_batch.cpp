
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Primitive_Batch::Primitive_Batch( e_render_prim_t render_prim )
{
	init( render_prim );
}

void Primitive_Batch::init( e_render_prim_t render_prim )
{
	vao[ e_draw_call::opaque ].init( this, render_prim );
	vao[ e_draw_call::opaque ].reset();

	vao[ e_draw_call::transparent ].init( this, render_prim );
	vao[ e_draw_call::transparent ].reset();
}

void Primitive_Batch::add_quad( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2, const Render_Vertex* v3 )
{
	auto alpha = ( v0->a + v1->a + v2->a + v3->a );
	auto draw_call = ( e_draw_call::transparent * ( alpha != 4.f ) );

	add_vert( draw_call, texture, v0 );
	add_vert( draw_call, texture, v1 );
	add_vert( draw_call, texture, v2 );
	add_vert( draw_call, texture, v3 );

	if( vao[ draw_call ].vb.vertices.num_objects_in_pool() >= vao[ draw_call ].vb.vertices.capacity() )
	{
		vao[ draw_call ].flush_and_reset( draw_call );
		//vao[ draw_call ].vb.reset();
	}
}

void Primitive_Batch::add_triangle( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2 )
{
	auto alpha = ( v0->a + v1->a + v2->a );
	auto draw_call = ( e_draw_call::transparent * ( alpha != 3.f ) );

	add_vert( draw_call, texture, v0 );
	add_vert( draw_call, texture, v1 );
	add_vert( draw_call, texture, v2 );

	if( vao[ draw_call ].vb.vertices.num_objects_in_pool() >= vao[ draw_call ].vb.vertices.capacity() )
	{
		vao[ draw_call ].flush_and_reset( draw_call );
		//vao[ draw_call ].vb.reset();
	}
}

void Primitive_Batch::add_line( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1 )
{
	auto alpha = ( v0->a + v1->a );
	auto draw_call = ( e_draw_call::transparent * ( alpha != 2.f ) );

	add_vert( draw_call, texture, v0 );
	add_vert( draw_call, texture, v1 );

	if( vao[ draw_call ].vb.vertices.num_objects_in_pool() >= vao[ draw_call ].vb.vertices.capacity() )
	{
		vao[ draw_call ].flush_and_reset( draw_call );
	}
}

void Primitive_Batch::add_point( Texture_Asset* texture, const Render_Vertex* v0 )
{
	auto alpha = ( v0->a );
	auto draw_call = ( e_draw_call::transparent * ( alpha != 1.f ) );

	add_vert( draw_call, texture, v0 );

	if( vao[ draw_call ].vb.vertices.num_objects_in_pool() >= vao[ draw_call ].vb.vertices.capacity() )
	{
		vao[ draw_call ].flush_and_reset( draw_call );
	}
}

bool Primitive_Batch::is_empty()
{
	return vao[ e_draw_call::opaque ].vb.vertices.empty() and vao[ e_draw_call::transparent ].vb.vertices.empty();
}

void Primitive_Batch::add_vert( e_draw_call_t draw_call, Texture_Asset* texture, const Render_Vertex* render_vert )
{
	auto texture_id = vao[ draw_call ].vb.assign_texture_slot( texture );

	// get a new render_vertex from the pool

	Render_Vertex* rvtx = vao[ draw_call ].vb.vertices.get_next();
	*rvtx = *render_vert;

	// multiply the current modelview matrix against the vertex being rendered.
	//
	// until this point, the vertex has been in object space. this
	// moves it into world space.

	auto transformed_vtx = g_engine->opengl_mgr.top_matrix->transform_vec2( Vec2( rvtx->x, rvtx->y ) );

	// update the position to the transformed position

	rvtx->x = transformed_vtx.x;
	rvtx->y = transformed_vtx.y;

	// fill in the details

	rvtx->texture_id = texture_id;
	rvtx->pick_id = Render::state->pick_id;
}

// ----------------------------------------------------------------------------
// a group of 4 render_batch objects, one for each kind of primitive the
// engine can draw. this is a convenience as it allows you to throw primitives
// at the renderer and it handles the details for you.

void Primitive_Batch_Group::init()
{
	batches[ e_render_prim::quad ].init( e_render_prim::quad );
	batches[ e_render_prim::triangle ].init( e_render_prim::triangle );
	batches[ e_render_prim::line ].init( e_render_prim::line );
	batches[ e_render_prim::point ].init( e_render_prim::point );
}

bool Primitive_Batch_Group::is_empty()
{
	for( auto& b : batches )
	{
		if( !b.is_empty() )
		{
			return false;
		}
	}

	return true;
}

void Primitive_Batch_Group::flush_and_reset( e_draw_call_t draw_call )
{
	for( auto& b : batches )
	{
		b.vao[ draw_call ].flush_and_reset( draw_call );
	}
}

void Primitive_Batch_Group::flush_and_reset_internal( e_draw_call_t draw_call )
{
	for( auto& b : batches )
	{
		b.vao[ draw_call ].flush_and_reset_internal( draw_call );
	}
}

void Primitive_Batch_Group::add_quad( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2, const Render_Vertex* v3 )
{
	batches[ e_render_prim::quad ].add_quad( texture, v0, v1, v2, v3 );
}

void Primitive_Batch_Group::add_triangle( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2 )
{
	batches[ e_render_prim::triangle ].add_triangle( texture, v0, v1, v2 );
}

void Primitive_Batch_Group::add_line( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1 )
{
	batches[ e_render_prim::line ].add_line( texture, v0, v1 );
}

void Primitive_Batch_Group::add_point( Texture_Asset* texture, const Render_Vertex* v0 )
{
	batches[ e_render_prim::point ].add_point( texture, v0 );
}

// lets you get a texture slot for a specific texture without having to actually
// draw anything. useful for when you want a shader to be able to access a
// texture that isn't attached to geometry, like a LUT or a mask.

size_t Primitive_Batch_Group::assign_texture_slot_manual( Texture_Asset* texture )
{
	// this is making wild assumptions about which batch you're referring
	// drawing from and which draw call pass. it's fine for now but this might
	// need changing in the future.

	return Render::state->batch_render_target
		->batches[ e_render_prim::quad ]
		.vao[ e_draw_call::opaque ]
		.vb.assign_texture_slot( texture );
}

}
