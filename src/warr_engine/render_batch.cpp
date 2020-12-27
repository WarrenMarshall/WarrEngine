
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_render_batch::w_render_batch( e_render_prim render_prim )
{
	vertex_array_object = std::make_unique<w_vertex_array_object>( this, render_prim );
    vertex_array_object->reset();
}

w_render_batch::~w_render_batch()
{
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2, const w_render_vertex& v3 )
{
	vertex_array_object->check_draw_and_reset();

	add_vert( tex, v0 );
	add_vert( tex, v1 );
	add_vert( tex, v2 );
	add_vert( tex, v3 );
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2 )
{
	vertex_array_object->check_draw_and_reset();

	add_vert( tex, v0 );
	add_vert( tex, v1 );
	add_vert( tex, v2 );
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1 )
{
	vertex_array_object->check_draw_and_reset();

	add_vert( tex, v0 );
	add_vert( tex, v1 );
}

void w_render_batch::add_primitive( const a_texture* tex, const w_render_vertex& v0 )
{
	vertex_array_object->check_draw_and_reset();

	add_vert( tex, v0 );
}

void w_render_batch::draw_and_reset()
{
	vertex_array_object->draw_and_reset();
}

bool w_render_batch::is_empty()
{
	return ( vertex_array_object->vertex_buffer->vertices.size() == 0 );
}

void w_render_batch::add_vert( const a_texture* tex, const w_render_vertex& render_vert )
{
	// multiply the current modelview matrix against the vertex being rendered.
	//
	// until this point, the vertex has been in model coordinate space. this
	// moves it into world space.

	w_vec2 vtx = OPENGL->top()->transform_vec2( w_vec2( render_vert.x, render_vert.y ) );

	// snap to pixel position

	if( RENDER->rs_snap_to_pixel )
	{
		vtx.x = snap_to_pixel( vtx.x );
		vtx.y = snap_to_pixel( vtx.y );
	}

	w_render_vertex rv = render_vert;

	// update the position to the transformed position
	rv.x = vtx.x;
	rv.y = vtx.y;

	// find a texture slot for the requested texture
	rv.t = static_cast<float>( vertex_array_object->assign_texture_slot( tex ) );

	// add the render_vert to the vertex list
	vertex_array_object->vertex_buffer->vertices.emplace_back( std::move( rv ) );
}
