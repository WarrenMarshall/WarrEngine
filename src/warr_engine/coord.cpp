#include "master_pch.h"
#include "master_header.h"

w_vec2 w_coord::window_to_virtual( const w_vec2& wpos )
{
	float scale = ( viewport_w / engine->window->viewport_pos_sz.w );

	return
	{
		( wpos.x - engine->window->viewport_pos_sz.x ) * scale,
		( wpos.y - engine->window->viewport_pos_sz.y ) * scale
	};
}

w_vec2 w_coord::window_to_ui( const w_vec2& wpos )
{
	auto vpos = window_to_virtual( wpos );
	return virtual_to_ui( vpos );
}

w_vec2 w_coord::window_to_world( const w_vec2& wpos, const w_entity* camera )
{
	auto vpos = window_to_virtual( wpos );
	return virtual_to_world( vpos, camera );
}

w_vec2 w_coord::virtual_to_ui( const w_vec2& vpos )
{
	return
	{
		vpos.x / ui_scale,
		vpos.y / ui_scale
	};
}

w_vec2 w_coord::ui_to_virtual( const w_vec2& uipos )
{
	return
	{
		uipos.x * ui_scale,
		uipos.y * ui_scale
	};
}

w_vec2 w_coord::virtual_to_world( const w_vec2& vpos, const w_entity* camera )
{
	w_vec2 wpos = vpos;

	if( camera )
	{
		wpos = camera->get_transform()->inverse_transform( vpos );
	}

	return wpos;
}

w_vec2 w_coord::world_to_virtual( const w_vec2& wpos, const w_entity* camera )
{
	w_vec2 vpos = wpos;

	if( camera )
	{
		vpos = camera->get_transform()->transform( vpos );
	}

	return vpos;
}

w_vec2 w_coord::ui_to_world( const w_vec2& uipos, const w_entity* camera )
{
	auto vpos = w_coord::ui_to_virtual( uipos );
	auto wpos = w_coord::virtual_to_world( vpos, camera );
	return wpos;
}

w_vec2 w_coord::world_to_ui( const w_vec2& wpos, const w_entity* camera )
{
	auto vpos = w_coord::world_to_virtual( wpos, camera );
	auto uipos = w_coord::virtual_to_ui( vpos );
	return uipos;
}
