
namespace war
{

struct render
{
	static palette_asset palette;

	// rendering batches that are recreated every frame. the bulk of the
	// rendered geo goes through these batches.
	render_batch_group dynamic_batches;

	// a value from 0.0-1.0 that represents how near we are to the NEXT update
	// tick. this is used to interpolate/predict rendering for smooth movement
	// and rotations even at low FTS values.

	float frame_interpolate_pct = 0.f;

#ifndef _FINAL_RELEASE
	struct
	{
		// if true, we are logging renderer information about the current frame
		// to the log file. this is useful for debugging things like the
		// batching system and to get a look at how many draw calls we're using
		// and what they are composed of.
		bool single_frame_log = false;
		bool entity_info_log = false;

		// if true, draw the extra debug information like box2d primitives and mesh wireframes
		bool draw_debug_info = false;

		bool is_single_frame_logging()
		{
			return single_frame_log;
		}

		bool is_entity_info_logging()
		{
			return entity_info_log;;
		}

		bool is_drawing_debug_info()
		{
			return draw_debug_info;
		}
	} debug;
#endif

	// circle sample points are stored in a unit circle
	constexpr static int circle_sample_points_max = 16;
	std::vector<vec2> circle_sample_points;

	void init();
	void init_set_up_default_palette();
	void init_generate_circle_sample_points();

	static void draw_quad( texture_asset* texture, const vec2& dst );
	static void draw_quad( texture_asset* texture, const rect& dst );

	static auto get_circle_start_end_indices( e_corner corner );
	static void draw_circle( const vec2& origin, float radius, e_corner corner = corner::all );
	static void draw_filled_circle( const vec2& origin, float radius, e_corner corner = corner::all );

	static void draw_rect( const rect& dst );
	static void draw_filled_rect( const rect& dst );

	static void draw_rounded_rect( const rect& dst, float radius );
	static void draw_rounded_filled_rect( const rect& dst, float radius );

	static void draw_triangle( const vec2& v0, const vec2& v1, const vec2& v2 );
	static void draw_filled_triangle( const vec2& v0, const vec2& v1, const vec2& v2 );

	static void draw_line( const vec2& start, const vec2& end );
	static void draw_lines( const std::vector<vec2>& list_of_verts );
	static void draw_line_loop( const rect& rc );
	static void draw_line_loop( const std::vector<vec2>& list_of_verts );

	static void draw_point( const vec2& pos );

	static void draw_mesh( mesh_asset* mesh );
	static void draw_sliced( const slice_def_asset* slice_def, const rect& dst );
	static void draw_sprite( texture_asset* texture, const vec2& dst );
	static vec2 draw_string( const std::string& text, const vec2& pos );
	static vec2 draw_string( const std::vector<std::string>& text, const vec2& rc );
	static vec2 draw_string( const std::vector<std::string>& text, const rect& rc );
	static void draw_tiled( texture_asset* texture, const rect& dst );
	static void draw_tile_map( tile_set_asset* tile_set, tile_map_asset* tile_map, const vec2& pos );
	static void draw_crosshair( vec2 pos );

	static std::vector<std::string> wrap_string_to_width( std::string_view text, float width );

	static void draw_world_axis();

	void begin_frame();
	void end_frame();

	[[nodiscard]] float calc_interpolated_per_sec_value( float current_value, float step_per_second ) const;

	// ----------------------------------------------------------------------------

	void clear_render_state_stack();

	std::deque<render_state> render_states;
	static render_state* state;

	render* push();
	render* pop();

	[[nodiscard]] static int sample_pick_id_at( vec2 viewport_click_pos );
};

}
