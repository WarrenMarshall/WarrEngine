
#pragma once

// ----------------------------------------------------------------------------

struct w_physics_query final
{
	static [[nodiscard]] bool trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask );
	static [[nodiscard]] bool trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_simple* hit_result );
	static [[nodiscard]] bool trace_closest( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_closest* hit_result );
	static [[nodiscard]] bool trace_all( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_all* hit_result );

	static [[nodiscard]] bool point_check_simple( w_vec2 pos, bitflags layer_mask );
	static [[nodiscard]] bool point_check_simple( w_vec2 pos, bitflags layer_mask, w_query_first* hit_result );
	static [[nodiscard]] bool point_check_all( w_vec2 pos, bitflags layer_mask, w_query_all* hit_result );
};
