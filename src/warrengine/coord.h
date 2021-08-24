
namespace war
{

struct coord_system final
{
	// ----------------------------------------------------------------------------
	// position transforms between spaces

	[[nodiscard]] static vec2 window_to_viewport_pos( const vec2& window_pos );

	[[nodiscard]] static vec2 viewport_to_world_pos( const vec2& viewport_pos );
	[[nodiscard]] static vec2 viewport_to_ui_pos( const vec2& viewport_pos );

	[[nodiscard]] static vec2 world_to_viewport_pos( const vec2& world_pos );
	[[nodiscard]] static vec2 ui_to_viewport_pos( const vec2& ui_pos );

	// ----------------------------------------------------------------------------
	// multi-step convenience functions

	[[nodiscard]] static vec2 window_to_world_pos( const vec2& window_pos );
	[[nodiscard]] static vec2 window_to_ui_pos( const vec2& window_pos );
	[[nodiscard]] static vec2 world_to_ui_pos( const vec2& world_pos );

	// ----------------------------------------------------------------------------
	// vector transforms (directions/deltas)
	//
	// for transforming movement rather than position.

	[[nodiscard]] static vec2 window_to_viewport_vec( const vec2& window_delta );
	[[nodiscard]] static vec2 viewport_to_world_vec( const vec2& viewport_delta );
	[[nodiscard]] static vec2 window_to_world_vec( const vec2& window_delta );
};

}
