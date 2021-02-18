#include "master_pch.h"
#include "master_header.h"

w_vec2 w_coord::window_to_virtual( const w_vec2& v )
{
	float ratio = ( v_window_w / engine->window->viewport_pos_sz.w );

	return
	{
		( (float)v.x - engine->window->viewport_pos_sz.x ) * ratio,
		( (float)v.y - engine->window->viewport_pos_sz.y ) * ratio
	};
}

w_vec2 w_coord::virtual_to_ui( const w_vec2& v )
{
	return
	{
		(float)v.x / ui_window_scale,
		(float)v.y / ui_window_scale
	};
}

w_vec2 w_coord::virtual_to_camera( const w_vec2& v, const w_entity* camera )
{
	return camera->get_transform()->inverse_transform( v );
}
