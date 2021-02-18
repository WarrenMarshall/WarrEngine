#include "master_pch.h"
#include "master_header.h"

w_vec2 w_coord::window_to_virtual( const w_vec2& wpos )
{
	float scale = ( v_window_w / engine->window->viewport_pos_sz.w );

	return
	{
		( wpos.x - engine->window->viewport_pos_sz.x ) * scale,
		( wpos.y - engine->window->viewport_pos_sz.y ) * scale
	};
}

w_vec2 w_coord::virtual_to_ui( const w_vec2& vpos )
{
	return
	{
		vpos.x / ui_window_scale,
		vpos.y / ui_window_scale
	};
}

w_vec2 w_coord::virtual_to_world( const w_vec2& vpos, const w_entity* camera )
{
	// if no camera is provided, then there is no difference between a virtual
	// position and a world position
	//
	// otherwise, use the camera transform to figure out where the virtual
	// position is in the world

	return
		camera
		? camera->get_transform()->inverse_transform( vpos )
		: vpos;
}
