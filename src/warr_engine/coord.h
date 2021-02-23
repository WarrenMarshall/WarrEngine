#pragma once

struct w_coord
{
	// ----------------------------------------------------------------------------
	// position transforms (locations)

	[[nodiscard]] static w_vec2 window_to_viewport_pos( const w_vec2& window_pos );

	[[nodiscard]] static w_vec2 viewport_to_world_pos( const w_vec2& viewport_pos, const w_entity* camera );
	[[nodiscard]] static w_vec2 viewport_to_ui_pos( const w_vec2& viewport_pos );

	[[nodiscard]] static w_vec2 world_to_viewport_pos( const w_vec2& viewport_pos, const w_entity* camera );
	[[nodiscard]] static w_vec2 ui_to_viewport_pos( const w_vec2& viewport_pos );

	// ----------------------------------------------------------------------------
	// multi-step functions for convenience

	[[nodiscard]] static w_vec2 window_to_world_pos( const w_vec2& window_pos, const w_entity* camera );
	[[nodiscard]] static w_vec2 window_to_ui_pos( const w_vec2& window_pos );

	// ----------------------------------------------------------------------------
	// vector transforms (directions/deltas)

	[[nodiscard]] static w_vec2 window_to_viewport_vec( const w_vec2& window_delta );
	[[nodiscard]] static w_vec2 viewport_to_world_vec( const w_vec2& viewport_delta, const w_entity* camera );
	[[nodiscard]] static w_vec2 window_to_world_vec( const w_vec2& window_delta, const w_entity* camera );
};

