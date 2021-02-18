#pragma once

struct w_coord
{
	_NODISCARD static w_vec2 window_to_virtual( const w_vec2& v );
	_NODISCARD static w_vec2 virtual_to_ui( const w_vec2& v );
	_NODISCARD static w_vec2 virtual_to_camera( const w_vec2& v, const w_entity* camera );
};
