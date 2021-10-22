
namespace war
{

struct Render final
{
	static Palette_Asset palette;
	Texture_Asset* tex_lut = nullptr;

	// rendering batches that are recreated every frame. the bulk of the
	// rendered geo goes through these batches.
	Primitive_Batch_Group dynamic_batches;

	// a value from 0.0-1.0 that represents how near we are to the NEXT update
	// tick. this is used to interpolate/predict rendering for smooth movement
	// and rotations even at low FTS values.

	f32 frame_interpolate_pct = 0.f;

#ifndef _RELEASE
	struct
	{
		// if true, we are logging renderer information about the current frame
		// to the log file. this is useful for debugging things like the
		// batching system and to get a look at how many draw calls we're using
		// and what they are composed of.
		bool single_frame_log : 1 = false;
		bool entity_info_log : 1 = false;

		// if true, draw the extra debug information like box2d primitives and mesh wireframes
		bool draw_colliders : 1 = false;
		// if true, draws the nodes in the currently active quad_tree
		bool draw_spatial : 1 = false;

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
			return draw_colliders;
		}

		bool is_drawing_debug_spatial()
		{
			return draw_spatial;
		}
	} debug;
#endif

	// render state stack
	std::vector<Render_State> render_states;
	static Render_State* state;

	// circle sample points are stored in a unit circle
	constexpr static i32 circle_sample_points_max = 16;
	std::vector<Vec2> circle_sample_points;

	void init();

	static void draw_quad( const Texture_Asset* texture, const Vec2& dst );
	static void draw_quad( const Texture_Asset* texture, const Rect& dst );

	static auto get_circle_start_end_indices( e_corner corner );
	static void draw_circle( const Vec2& origin, f32 radius, e_corner corner = e_corner::all );
	static void draw_filled_circle( const Vec2& origin, f32 radius, e_corner corner = e_corner::all );

	static void draw_rect( const Rect& dst );
	static void draw_filled_rect( const Rect& dst );

	static void draw_rounded_rect( const Rect& dst, f32 radius );
	static void draw_rounded_filled_rect( const Rect& dst, f32 radius );

	static void draw_triangle( const Vec2& v0, const Vec2& v1, const Vec2& v2 );
	static void draw_filled_triangle( const Vec2& v0, const Vec2& v1, const Vec2& v2 );

	static void draw_line( const Vec2& start, const Vec2& end );
	static void draw_lines( const std::vector<Vec2>& list_of_verts );
	static void draw_line_loop( const Rect& rc );
	static void draw_line_loop( const std::vector<Vec2>& list_of_verts );

	static void draw_point( const Vec2& pos );

	static void draw_mesh( const Mesh_Asset* mesh );
	static void draw_sliced( const Slice_Def_Asset* slice_def, const Rect& dst );
	static void draw_sprite( const Texture_Asset* texture, const Vec2& dst );
	static Vec2 draw_string( const std::string& text, const Vec2& pos );
	static Vec2 draw_string( const std::vector<std::string>& text, const Rect& rc );
	static void draw_tiled( const Texture_Asset* texture, const Rect& dst );
	static void draw_tile_map( const Tile_Set_Asset* tile_set, const Tile_Map_Asset* tile_map, const Vec2& pos );
	static void draw_crosshair( Vec2 pos );

	static std::vector<std::string> wrap_string_to_width( std::string_view text, f32 width );

	static void draw_world_axis();

	void begin_frame();
	void end_frame();

	[[nodiscard]] f32 calc_interpolated_per_sec_value( f32 current_value, f32 step_per_second ) const;

	// ----------------------------------------------------------------------------

	void clear_render_state_stack();

	Render* push();
	Render* pop();

	[[nodiscard]] static i32 sample_pick_id_at( Vec2 viewport_click_pos );

	// forces a flushing of the render buffers that are currently in progress
	static void flush();
};

}
