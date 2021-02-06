
#pragma once

// ----------------------------------------------------------------------------

struct w_physics_query final
{
	_NODISCARD static bool trace_simple( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask );
	_NODISCARD static bool trace_simple( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_simple* hit_result );
	_NODISCARD static bool trace_closest( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_closest* hit_result );
	_NODISCARD static bool trace_all( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_all* hit_result );

	_NODISCARD static bool point_check_simple( const w_vec2& pos );
	_NODISCARD static bool point_check_simple( const w_vec2& pos, w_query_first* hit_result );
	_NODISCARD static bool point_check_all( const w_vec2& pos, w_query_all* hit_result );
};
