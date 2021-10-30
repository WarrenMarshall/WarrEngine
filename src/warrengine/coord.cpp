#include "master_pch.h"

namespace war
{

// ----------------------------------------------------------------------------

Vec2 Coord_System::window_to_viewport_pos( const Vec2& window_pos )
{
	f32 scale_w = ( viewport_w / g_engine->window.viewport_pos_sz.w );
	f32 scale_h = ( viewport_h / g_engine->window.viewport_pos_sz.h );

	return
	{
		( window_pos.x - g_engine->window.viewport_pos_sz.x ) * scale_w,
		( window_pos.y - g_engine->window.viewport_pos_sz.y ) * scale_h
	};
}

Vec2 Coord_System::viewport_to_world_pos( const Vec2& viewport_pos )
{
	Vec2 world_pos = viewport_pos;

	Matrix mtx_viewport_pivot;
	mtx_viewport_pivot.translate( g_engine->scene_mgr.get_viewport_pivot() );
	mtx_viewport_pivot.invert();
	mtx_viewport_pivot.transform_vec2( &world_pos );

	auto mtx = g_engine->scene_mgr.get_transform()->to_matrix();
	mtx.invert();
	mtx.transform_vec2( &world_pos );

	return world_pos;
}

Vec2 Coord_System::viewport_to_ui_pos( const Vec2& viewport_pos )
{
	return
	{
		viewport_pos.x / ui_scale,
		viewport_pos.y / ui_scale
	};
}

Vec2 Coord_System::world_to_viewport_pos( const Vec2& world_pos )
{
	Vec2 viewport_pos = world_pos;

	auto cam_mtx = g_engine->scene_mgr.get_transform()->to_matrix();
	cam_mtx.transform_vec2( &viewport_pos );

	Matrix mtx_viewport_pivot;
	mtx_viewport_pivot.translate( g_engine->scene_mgr.get_viewport_pivot() );
	mtx_viewport_pivot.transform_vec2( &viewport_pos );

	return viewport_pos;
}

Vec2 Coord_System::ui_to_viewport_pos( const Vec2& ui_pos )
{
	return
	{
		ui_pos.x * ui_scale,
		ui_pos.y * ui_scale
	};
}

// ----------------------------------------------------------------------------

Vec2 Coord_System::window_to_world_pos( const Vec2& window_pos )
{
	auto viewport_pos = Coord_System::window_to_viewport_pos( window_pos );
	auto world_pos = Coord_System::viewport_to_world_pos( viewport_pos );

	return world_pos;
}

Vec2 Coord_System::window_to_ui_pos( const Vec2& window_pos )
{
	auto viewport_pos = Coord_System::window_to_viewport_pos( window_pos );
	auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

	return ui_pos;
}

Vec2 Coord_System::world_to_ui_pos( const Vec2& world_pos )
{
	auto viewport_pos = Coord_System::world_to_viewport_pos( world_pos );
	auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

	return ui_pos;
}

// ----------------------------------------------------------------------------

Vec2 Coord_System::window_to_viewport_vec( const Vec2& window_delta )
{
	f32 scale_w = ( viewport_w / g_engine->window.viewport_pos_sz.w );
	f32 scale_h = ( viewport_h / g_engine->window.viewport_pos_sz.h );

	return
	{
		window_delta.x * scale_w,
		window_delta.y * scale_h
	};
}

Vec2 Coord_System::viewport_to_world_vec( const Vec2& viewport_delta )
{
	Vec2 world_delta = viewport_delta;

	auto angle_mtx = g_engine->scene_mgr.get_transform()->to_matrix_vec();
	angle_mtx.invert();
	angle_mtx.transform_vec2( &world_delta );

	return world_delta;
}

Vec2 Coord_System::window_to_world_vec( const Vec2& window_delta )
{
	auto viewport_vec = Coord_System::window_to_viewport_vec( window_delta );
	auto world_vec = Coord_System::viewport_to_world_vec( viewport_vec );

	return world_vec;
}

}
