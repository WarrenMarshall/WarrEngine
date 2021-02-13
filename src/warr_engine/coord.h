#pragma once

struct w_coord
{
	static w_vec2 window_pos_to_virtual_pos( const w_vec2& v );
	static w_vec2 virtual_pos_to_ui_pos( const w_vec2& v );
	static w_vec2 virtual_pos_to_camera_pos( const w_vec2& v, w_entity* camera );
};
