#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_vec2 w_coord::window_to_viewport_pos( const w_vec2& window_pos )
{
	float scale_w = ( viewport_w / engine->window->viewport_pos_sz.w );
	float scale_h = ( viewport_h / engine->window->viewport_pos_sz.h );

	return
	{
		( window_pos.x - engine->window->viewport_pos_sz.x ) * scale_w,
		( window_pos.y - engine->window->viewport_pos_sz.y ) * scale_h
	};
}

w_vec2 w_coord::viewport_to_world_pos( const w_vec2& viewport_pos, const w_entity* camera )
{
	w_vec2 world_pos = viewport_pos;

	if( camera )
	{
		world_pos = camera->get_transform()->inverse_transform( viewport_pos );
	}

	return world_pos;
}

w_vec2 w_coord::viewport_to_ui_pos( const w_vec2& viewport_pos )
{
	return
	{
		viewport_pos.x / ui_scale,
		viewport_pos.y / ui_scale
	};
}

w_vec2 w_coord::world_to_viewport_pos( const w_vec2& world_pos, const w_entity* camera )
{
	w_vec2 viewport_pos = world_pos;

	if( camera )
	{
		viewport_pos = camera->get_transform()->transform( world_pos );
	}

	return viewport_pos;
}

w_vec2 w_coord::ui_to_viewport_pos( const w_vec2& ui_pos )
{
	return
	{
		ui_pos.x * ui_scale,
		ui_pos.y * ui_scale
	};
}

// ----------------------------------------------------------------------------

w_vec2 w_coord::window_to_viewport_vec( const w_vec2& window_pos )
{
	float scale_w = ( viewport_w / engine->window->viewport_pos_sz.w );
	float scale_h = ( viewport_h / engine->window->viewport_pos_sz.h );

	return
	{
		window_pos.x * scale_w,
		window_pos.y * scale_h
	};
}
