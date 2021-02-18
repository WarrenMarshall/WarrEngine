#pragma once

struct w_coord
{
	[[nodiscard]] static w_vec2 window_to_virtual( const w_vec2& wpos );
	[[nodiscard]] static w_vec2 virtual_to_ui( const w_vec2& vpos );
	[[nodiscard]] static w_vec2 virtual_to_world( const w_vec2& vpos, const w_entity* camera );
};
