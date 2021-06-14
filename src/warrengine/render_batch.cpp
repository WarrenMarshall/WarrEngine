
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

render_batch::render_batch( e_render_prim render_prim )
{
	init( render_prim );
}

void render_batch::init( e_render_prim render_prim )
{
	vao.init( this, render_prim );
	vao.reset();
}

void render_batch::draw()
{
	vao.draw();
}

void render_batch::add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 )
{
	add_vert( texture, v0 );
	add_vert( texture, v1 );
	add_vert( texture, v2 );
	add_vert( texture, v3 );
}

void render_batch::add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 )
{
	add_vert( texture, v0 );
	add_vert( texture, v1 );
	add_vert( texture, v2 );
}

void render_batch::add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 )
{
	add_vert( texture, v0 );
	add_vert( texture, v1 );
}

void render_batch::add_point( texture_asset* texture, const render_vertex* v0 )
{
	add_vert( texture, v0 );
}

bool render_batch::is_empty()
{
	assert( vao.vb );

	return vao.vb->vertices.empty();
}

void render_batch::add_vert( texture_asset* texture, const render_vertex* render_vert )
{
	// get a new render_vertex from the pool

	render_vertex* rvtx = vao.vb->vertices.get_next();
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

	rvtx->texture_id = vao.vb->assign_texture_slot( texture );
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

void render_batch_group::flush_and_reset()
{
	for( auto& b : batches )
	{
		b.vao.flush_and_reset();
	}
}

void render_batch_group::flush_and_reset_internal()
{
	for( auto& b : batches )
	{
		b.vao.flush_and_reset_internal();
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
