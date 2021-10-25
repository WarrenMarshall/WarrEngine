
namespace war::box2d
{

struct Query final
{
	[[nodiscard]] static bool trace_quick( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask );
	[[nodiscard]] static bool trace_quick( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask, Raycast_Simple* hit_result );
	[[nodiscard]] static bool trace_closest( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask, Raycast_Closest* hit_result );
	[[nodiscard]] static bool trace_all( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask, Raycast_All* hit_result );

	[[nodiscard]] static bool point_check_simple( const Vec2& pos );
	[[nodiscard]] static bool point_check_simple( const Vec2& pos, Touching_First* hit_result );
	[[nodiscard]] static bool point_check_all( const Vec2& pos, touching_all* hit_result );
};

}
