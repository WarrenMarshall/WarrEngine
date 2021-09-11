
namespace war
{

struct Render
{
	static Palette_Asset palette;

	// rendering batches that are recreated every frame. the bulk of the
	// rendered geo goes through these batches.
	Primitive_Batch_Group dynamic_batches;

	// a value from 0.0-1.0 that represents how near we are to the NEXT update
	// tick. this is used to interpolate/predict rendering for smooth movement
	// and rotations even at low FTS values.

	float_t frame_interpolate_pct = 0.f;

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
	constexpr static int32_t circle_sample_points_max = 16;
	std::vector<Vec2> circle_sample_points;

	void init();
	void init_set_up_default_palette();
	void init_generate_circle_sample_points();

	static void draw_quad( Texture_Asset* texture, const Vec2& dst );
	static void draw_quad( Texture_Asset* texture, const Rect& dst );

	static auto get_circle_start_end_indices( e_corner_t corner );
	static void draw_circle( const Vec2& origin, float_t radius, e_corner_t corner = e_corner::all );
	static void draw_filled_circle( const Vec2& origin, float_t radius, e_corner_t corner = e_corner::all );

	static void draw_rect( const Rect& dst );
	static void draw_filled_rect( const Rect& dst );

	static void draw_rounded_rect( const Rect& dst, float_t radius );
	static void draw_rounded_filled_rect( const Rect& dst, float_t radius );

	static void draw_triangle( const Vec2& v0, const Vec2& v1, const Vec2& v2 );
	static void draw_filled_triangle( const Vec2& v0, const Vec2& v1, const Vec2& v2 );

	static void draw_line( const Vec2& start, const Vec2& end );
	static void draw_lines( const std::vector<Vec2>& list_of_verts );
	static void draw_line_loop( const Rect& rc );
	static void draw_line_loop( const std::vector<Vec2>& list_of_verts );

	static void draw_point( const Vec2& pos );

	static void draw_mesh( Mesh_Asset* mesh );
	static void draw_sliced( const Slice_Def_Asset* slice_def, const Rect& dst );
	static void draw_sprite( Texture_Asset* texture, const Vec2& dst );
	static Vec2 draw_string( const std::string& text, const Vec2& pos );
	static Vec2 draw_string( const std::vector<std::string>& text, const Rect& rc );
	static void draw_tiled( Texture_Asset* texture, const Rect& dst );
	static void draw_tile_map( Tile_Set_Asset* tile_set, Tile_Map_Asset* tile_map, const Vec2& pos );
	static void draw_crosshair( Vec2 pos );

	static std::vector<std::string> wrap_string_to_width( std::string_view text, float_t width );

	static void draw_world_axis();

	void begin_frame();
	void end_frame();

	[[nodiscard]] float_t calc_interpolated_per_sec_value( float_t current_value, float_t step_per_second ) const;

	// ----------------------------------------------------------------------------

	void clear_render_state_stack();

	std::vector<Render_State> render_states;
	static Render_State* state;

	Render* push();
	Render* pop();

	[[nodiscard]] static int32_t sample_pick_id_at( Vec2 viewport_click_pos );
};

}
