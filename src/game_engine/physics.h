
#pragma once

// ----------------------------------------------------------------------------

struct w_physics
{
	static bool trace_simple( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask );
	static bool trace_simple( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_simple* hit_result );
	static bool trace_closest( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_closest* hit_result );
	static bool trace_all( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_all* hit_result );

	static bool point_check_simple( w_vec2 pos, e_collision_layer layer_mask, w_query_first* hit_result );
	static bool point_check_all( w_vec2 pos, e_collision_layer layer_mask, w_query_all* hit_result );
};
