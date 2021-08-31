
namespace war
{

struct Coord_System
{
	// ----------------------------------------------------------------------------
	// position transforms between spaces

	[[nodiscard]] static Vec2 window_to_viewport_pos( const Vec2& window_pos );

	[[nodiscard]] static Vec2 viewport_to_world_pos( const Vec2& viewport_pos );
	[[nodiscard]] static Vec2 viewport_to_ui_pos( const Vec2& viewport_pos );

	[[nodiscard]] static Vec2 world_to_viewport_pos( const Vec2& world_pos );
	[[nodiscard]] static Vec2 ui_to_viewport_pos( const Vec2& ui_pos );

	// ----------------------------------------------------------------------------
	// multi-step convenience functions

	[[nodiscard]] static Vec2 window_to_world_pos( const Vec2& window_pos );
	[[nodiscard]] static Vec2 window_to_ui_pos( const Vec2& window_pos );
	[[nodiscard]] static Vec2 world_to_ui_pos( const Vec2& world_pos );

	// ----------------------------------------------------------------------------
	// vector transforms (directions/deltas)
	//
	// for transforming movement rather than position.

	[[nodiscard]] static Vec2 window_to_viewport_vec( const Vec2& window_delta );
	[[nodiscard]] static Vec2 viewport_to_world_vec( const Vec2& viewport_delta );
	[[nodiscard]] static Vec2 window_to_world_vec( const Vec2& window_delta );
};

}
