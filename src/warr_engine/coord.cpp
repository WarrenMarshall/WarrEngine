#include "master_pch.h"
#include "master_header.h"

w_vec2 w_coord::window_pos_to_virtual_pos( const w_vec2& v )
{
	float ratio = ( v_window_w / engine->window->viewport_pos_sz.w );

	return
	{
		( (float)v.x - engine->window->viewport_pos_sz.x ) * ratio,
		( (float)v.y - engine->window->viewport_pos_sz.y ) * ratio
	};
}

w_vec2 w_coord::virtual_pos_to_ui_pos( const w_vec2& v )
{
	return
	{
		(float)v.x / ui_window_scale,
		(float)v.y / ui_window_scale
	};
}

w_vec2 w_coord::virtual_pos_to_camera_pos( const w_vec2& v, w_entity* camera )
{
	return camera->get_transform()->inv_transform_pos( v );
}
