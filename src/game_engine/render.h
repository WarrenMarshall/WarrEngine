#pragma once

struct w_render_stats
{
	w_render_stats();

	std::unique_ptr<w_timer> stat_timer = nullptr;

	w_accum_value frame_count;

	w_accum_value render_buffers;
	w_accum_value render_vertices;
	w_accum_value render_indices;
	w_accum_value frame_times_ms;
	w_accum_value num_entities;

	std::string stat_custom_string;

	void update();
};

// ----------------------------------------------------------------------------

struct w_render
{
	/*
		a value from 0.0-1.0 that represents how near we are to the NEXT update
		tick. this is used to interpolate/predict rendering for smooth
		movement and rotations even at low FTS values.
	*/
	float frame_interpolate_pct = 0.0f;

	// if this is set, we use it for things like setting up the view matrix.
	w_camera* current_camera = nullptr;

	/*
		batch rendering
	*/
	a_texture* current_texture = nullptr;
	std::unique_ptr<w_render_buffer> master_render_buffer = nullptr;
	void draw_master_buffer();
	void maybe_draw_master_buffer( a_texture* texture );

	// ----------------------------------------------------------------------------
	// the current render state stacks

	std::basic_string<w_color> rs_color_stack;	// 3 floats per color
	std::basic_string<float> rs_alpha_stack;
	std::basic_string<float> rs_scale_stack;
	std::basic_string<float> rs_angle_stack;
	std::basic_string<e_align> rs_align_stack;

	float zdepth = 0.0f;
	float zdepth_nudge_accum = 0.0f;;

	w_render* begin();
	void end();

	w_render* push_rgb( const w_color& color );
	w_render* pop_rgb();
	w_render* push_rgba( const w_color& color );
	w_render* push_rgba( const w_color& color, const float alpha );
	w_render* pop_rgba();
	w_render* push_alpha( const float alpha );
	w_render* pop_alpha();
	w_render* push_scale( const float scale );
	w_render* push_angle( const float angle );
	w_render* push_align( const e_align& align );
	w_render* push_depth( const float depth );
	w_render* push_depth_nudge( const float nudge = zdepth_nudge );

	// ----------------------------------------------------------------------------

	bool show_stats = false;
	bool show_physics_debug = true;

	// circle sample points are stored in a unit circle
	static constexpr int circle_sample_points_max = 16;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	void init_projection_matrix() const;
	void init_view_matrix() const;
	void init_view_matrix_identity() const;

	w_render* draw_sprite( a_texture* tex, const w_vec2& dst );
	w_render* draw_sprite( const a_subtexture* subtex, const w_vec2& dst );
	w_render* draw( a_texture* tex, const w_rect& dst );
	w_render* draw( const a_subtexture* subtex, const w_rect& dst );
	w_render* draw_sliced( const a_9slice_def* slice_def, const w_rect& dst );
	w_render* draw_string( a_font* font, const std::string_view text, const w_rect& dst );
	w_render* draw_rectangle( const w_rect& dst );
	w_render* draw_filled_rectangle( const w_rect& dst );
	w_render* draw_line( const w_vec2& start, const w_vec2& end );
	w_render* draw_point( const w_vec2& pos );
	w_render* draw_circle( const w_vec2& origin, float radius );
	w_render* draw_world_axis();
	w_render* draw_stats();

	void begin_frame( float frame_interpolate_pct );
	void end_frame();

	[[nodiscard]] float calc_interpolated_per_sec_value( float current_value, float step_per_second ) const;

	w_render_stats stats;
};

