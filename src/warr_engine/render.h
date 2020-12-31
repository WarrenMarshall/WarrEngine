#pragma once

// ----------------------------------------------------------------------------

struct alignas( struct_alignment_for_cache ) w_render_state
{
	w_color color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float glow = 0.0f;
	w_vec2 scale = { 1.0f, 1.0f };
	float angle = 0.0f;
	e_align align = align::left;
	bool snap_to_pixel = true;
};

// making sure that these are small enough to fit into the cache nicely and
// that they are fast to copy around.

static_assert( sizeof( w_render_state ) <= 64 );
static_assert( std::is_trivially_assignable<w_render_state, w_render_state>() );
static_assert( std::is_trivially_copy_constructible<w_render_state>() );

// ----------------------------------------------------------------------------

struct w_render
{
	a_palette* palette = nullptr;

	w_color get_palette_color_from_idx( int idx );

	bool batches_are_empty();

	/*
		a value from 0.0-1.0 that represents how near we are to the NEXT update
		tick. this is used to interpolate/predict rendering for smooth
		movement and rotations even at low FTS values.
	*/
	float frame_interpolate_pct = 0.0f;

	/*
		batch rendering
	*/
	std::unique_ptr<w_render_batch> batch_quads = nullptr;
	std::unique_ptr<w_render_batch> batch_triangles = nullptr;
	std::unique_ptr<w_render_batch> batch_lines = nullptr;
	std::unique_ptr<w_render_batch> batch_points = nullptr;

	void draw_and_reset_all_batches();

	// palette support
	a_palette* current_palette = nullptr;
	w_color pal_color_from_idx( int idx );

	// ----------------------------------------------------------------------------
	// the current render state stacks

	std::vector<w_render_state> render_states;
	w_render_state* rs_top();
	w_render_state* rs_push();
	w_render_state* rs_push( w_render_state& rs);
	w_render_state* rs_pop();
	void rs_reset();

	float rs_z_depth = 0.0f;
	float rs_z_depth_nudge_accum = 0.0f;

#ifndef _FINALRELEASE
	bool single_frame_debugger = false;
	bool show_physics_debug = true;
#endif

	w_render* begin();
	w_render* end();

	w_render* push_rgb( const w_color& color );
	w_render* replace_rgb( const w_color& color );
	w_render* push_rgba( const w_color& color );
	w_render* replace_rgba( const w_color& color );
	w_render* push_rgba( const w_color& color, const float alpha );
	w_render* replace_rgba( const w_color& color, const float alpha );
	w_render* push_alpha( const float alpha );
	w_render* replace_alpha( const float alpha );
	w_render* push_glow( const float glow );
	w_render* push_scale( const w_vec2& scale );
	w_render* replace_scale( const w_vec2& scale );
	w_render* push_scale( const float scale );
	w_render* replace_scale( const float scale );
	w_render* push_angle( const float angle );
	w_render* replace_angle( const float angle );
	w_render* push_align( const e_align& align );
	w_render* replace_align( const e_align& align );
	w_render* push_depth( const float depth );
	w_render* push_depth_nudge( const float nudge = zdepth_nudge );
	w_render* push_snap_to_pixel( bool snap_to_pixel );
	w_render* push_render_state( w_render_state& rs );
	w_render* pop();

	// ----------------------------------------------------------------------------

	bool show_stats = false;
	std::vector<std::string> stat_lines;

	// circle sample points are stored in a unit circle
	static constexpr int circle_sample_points_max = 16;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	w_render* draw_mesh( a_mesh* mesh, const w_vec2& dst );
	w_render* draw_sprite( const a_texture* texture, const w_vec2& dst );
	w_render* draw( const a_texture* texture, const w_rect& dst );
	w_render* draw_sliced( const a_9slice_def* slice_def, const w_rect& dst );
	w_render* draw_string( const std::string_view text, const w_rect& dst );
	w_render* draw_string( a_font* font, const std::string_view text, const w_rect& dst );
	w_render* draw_rectangle( const w_rect& dst );
	w_render* draw_filled_rectangle( const w_rect& dst );
	w_render* draw_triangle( const w_vec2& v0, const w_vec2& v1 , const w_vec2& v2 );
	w_render* draw_filled_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 );
	w_render* draw_line( const w_vec2& start, const w_vec2& end );
	w_render* draw_point( const w_vec2& pos );
	w_render* draw_circle( const w_vec2& origin, float radius );
	w_render* draw_filled_circle ( const w_vec2& origin, float radius );
	w_render* draw_world_axis();
	w_render* draw_stats();

	void begin_frame();
	void end_frame();

	[[nodiscard]] float calc_interpolated_per_sec_value( float current_value, float step_per_second ) const;

	void bind_texture( int slot, a_src_texture* tex );
	w_render_stats stats;
};

