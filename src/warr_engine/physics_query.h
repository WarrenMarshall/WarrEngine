
#pragma once

// ----------------------------------------------------------------------------

struct w_physics_query final
{
	static [[nodiscard]] bool trace_simple( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask );
	static [[nodiscard]] bool trace_simple( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_simple* hit_result );
	static [[nodiscard]] bool trace_closest( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_closest* hit_result );
	static [[nodiscard]] bool trace_all( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_all* hit_result );

	static [[nodiscard]] bool point_check_simple( const w_vec2& pos, int layer_mask );
	static [[nodiscard]] bool point_check_simple( const w_vec2& pos, int layer_mask, w_query_first* hit_result );
	static [[nodiscard]] bool point_check_all( const w_vec2& pos, int layer_mask, w_query_all* hit_result );
};
