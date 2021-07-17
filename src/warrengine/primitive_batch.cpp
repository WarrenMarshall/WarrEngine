
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

primitive_batch::primitive_batch( e_render_prim render_prim )
{
	init( render_prim );
}

void primitive_batch::init( e_render_prim render_prim )
{
	vao[ draw_call::opaque ].init( this, render_prim );
	vao[ draw_call::opaque ].reset();

	vao[ draw_call::transparent ].init( this, render_prim );
	vao[ draw_call::transparent ].reset();
}

void primitive_batch::add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 )
{
	auto alpha = ( v0->a + v1->a + v2->a + v3->a );
	auto draw_call = ( draw_call::transparent * ( alpha != 4.0f ) );

	add_vert( draw_call, texture, v0 );
	add_vert( draw_call, texture, v1 );
	add_vert( draw_call, texture, v2 );
	add_vert( draw_call, texture, v3 );
}

void primitive_batch::add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 )
{
	auto alpha = ( v0->a + v1->a + v2->a );
	auto draw_call = ( draw_call::transparent * ( alpha != 3.0f ) );

	add_vert( draw_call, texture, v0 );
	add_vert( draw_call, texture, v1 );
	add_vert( draw_call, texture, v2 );
}

void primitive_batch::add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 )
{
	auto alpha = ( v0->a + v1->a );
	auto draw_call = ( draw_call::transparent * ( alpha != 2.0f ) );

	add_vert( draw_call, texture, v0 );
	add_vert( draw_call, texture, v1 );
}

void primitive_batch::add_point( texture_asset* texture, const render_vertex* v0 )
{
	auto alpha = ( v0->a );
	auto draw_call = ( draw_call::transparent * ( alpha != 1.0f ) );

	add_vert( draw_call, texture, v0 );
}

bool primitive_batch::is_empty()
{
	return vao[ draw_call::opaque ].vb->vertices.empty() and vao[ draw_call::transparent ].vb->vertices.empty();
}

void primitive_batch::add_vert( e_draw_call draw_call, texture_asset* texture, const render_vertex* render_vert )
{
	// get a new render_vertex from the pool

	render_vertex* rvtx = vao[ draw_call ].vb->vertices.get_next();
	*rvtx = *render_vert;

	// multiply the current modelview matrix against the vertex being rendered.
	//
	// until this point, the vertex has been in object space. this
	// moves it into world space.

	auto transformed_vtx = g_engine->render_api.top_matrix->transform_vec2( vec2( rvtx->x, rvtx->y ) );

	// update the position to the transformed position

	rvtx->x = transformed_vtx.x;
	rvtx->y = transformed_vtx.y;

	// fill in the details

	rvtx->texture_id = vao[ draw_call ].vb->assign_texture_slot( texture );
	rvtx->pick_id = render::state->pick_id;
}

// ----------------------------------------------------------------------------
// a group of 4 render_batch objects, one for each kind of primitive the
// engine can draw. this is a convenience as it allows you to throw primitives
// at the renderer and it handles the details for you.

void render_batch_group::init()
{
	batches[ render_prim::quad ].init( render_prim::quad );
	batches[ render_prim::triangle ].init( render_prim::triangle );
	batches[ render_prim::line ].init( render_prim::line );
	batches[ render_prim::point ].init( render_prim::point );
}

bool render_batch_group::is_empty()
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

void render_batch_group::flush_and_reset( e_draw_call draw_call )
{
	for( auto& b : batches )
	{
		b.vao[ draw_call ].flush_and_reset( draw_call );
	}
}

void render_batch_group::flush_and_reset_internal( e_draw_call draw_call )
{
	for( auto& b : batches )
	{
		b.vao[ draw_call ].flush_and_reset_internal( draw_call );
	}
}

void render_batch_group::add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 )
{
	batches[ render_prim::quad ].add_quad( texture, v0, v1, v2, v3 );
}

void render_batch_group::add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 )
{
	batches[ render_prim::triangle ].add_triangle( texture, v0, v1, v2 );
}

void render_batch_group::add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 )
{
	batches[ render_prim::line ].add_line( texture, v0, v1 );
}

void render_batch_group::add_point( texture_asset* texture, const render_vertex* v0 )
{
	batches[ render_prim::point ].add_point( texture, v0 );
}

}
