#pragma once

struct w_coord
{
	[[nodiscard]] static w_vec2 window_to_viewport( const w_vec2& window_pos );

	[[nodiscard]] static w_vec2 viewport_to_world( const w_vec2& viewport_pos, const w_entity* camera );
	[[nodiscard]] static w_vec2 viewport_to_ui( const w_vec2& viewport_pos );

	[[nodiscard]] static w_vec2 world_to_viewport( const w_vec2& viewport_pos, const w_entity* camera );
	[[nodiscard]] static w_vec2 ui_to_viewport( const w_vec2& viewport_pos );
};

