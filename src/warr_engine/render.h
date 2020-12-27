#pragma once

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

	std::vector<w_color> rs_color_stack;
	std::vector<float> rs_alpha_stack;
	std::vector<float> rs_emissive_stack;
	std::vector<w_vec2> rs_scale_stack;
	std::vector<float> rs_angle_stack;
	std::vector<e_align> rs_align_stack;

	float rs_z_depth = 0.0f;
	float rs_z_depth_nudge_accum = 0.0f;

	bool rs_snap_to_pixel = true;

#ifndef _FINALRELEASE
	bool enable_frame_debugger = false;
	bool show_physics_debug = true;
#endif

	w_render* begin();
	w_render* end();
	void clear_render_states();

	w_render* push_rgb( const w_color& color );
	w_render* replace_rgb( const w_color& color );
	w_render* pop_rgb();
	w_render* push_rgba( const w_color& color );
	w_render* replace_rgba( const w_color& color );
	w_render* push_rgba( const w_color& color, const float alpha );
	w_render* replace_rgba( const w_color& color, const float alpha );
	w_render* pop_rgba();
	w_render* push_alpha( const float alpha );
	w_render* replace_alpha( const float alpha );
	w_render* pop_alpha();
	w_render* push_emissive( const float emissive );
	w_render* pop_emissive();
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

	// ----------------------------------------------------------------------------

	bool show_stats = false;
	std::vector<std::string> stat_lines;

	// circle sample points are stored in a unit circle
	static constexpr int circle_sample_points_max = 16;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	w_render* draw_mesh( a_mesh* mesh, const w_vec2& dst );
	w_render* draw_sprite( a_texture* tex, const w_vec2& dst );
	w_render* draw_sprite( const a_subtexture* subtex, const w_vec2& dst );
	w_render* draw( a_texture* tex, const w_rect& dst );
	w_render* draw( const a_subtexture* subtex, const w_rect& dst );
	w_render* draw_sliced( const a_9slice_def* slice_def, const w_rect& dst );
	w_render* draw_string( const std::string_view text, const w_rect& dst );
	w_render* draw_string( a_font* font, const std::string_view text, const w_rect& dst );
	w_render* draw_rectangle( const w_rect& dst );
	w_render* draw_filled_rectangle( const w_rect& dst );
	w_render* draw_line( const w_vec2& start, const w_vec2& end );
	w_render* draw_point( const w_vec2& pos );
	w_render* draw_circle( const w_vec2& origin, float radius );
	w_render* draw_world_axis();
	w_render* draw_stats();

	void begin_frame();
	void end_frame();

	[[nodiscard]] float calc_interpolated_per_sec_value( float current_value, float step_per_second ) const;

	void bind_texture( int slot, a_texture* tex );
	w_render_stats stats;
};

