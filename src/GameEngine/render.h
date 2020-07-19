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
	w_accum_value num_frames_rendered;

	std::string stat_custom_string;

	void update();
};

// ----------------------------------------------------------------------------

struct w_render
{
	w_render_stats stats;

	/*
		a value from 0.0-1.0 that represents how near we are to the NEXT update
		tick. this is used to interpolate/predict rendering for smooth
		movement and rotations even at low FTS values.
	*/
	float frame_interpolate_pct = 0.0f;

	std::stack<glm::mat4> modelview_stack;

	// ----------------------------------------------------------------------------
	// the current render state stacks

	int rs_color_count = 0;
	std::stack<w_color> rs_color_stack;

	int rs_alpha_count = 0;
	std::stack<float> rs_alpha_stack;

	int rs_scale_count = 0;
	std::stack<float> rs_scale_stack;
	
	int rs_align_count = 0;
	std::stack<e_align> rs_align_stack;
	
	int rs_depth_count = 0;
	std::stack<float> rs_depth_stack;

	w_render* begin();
	w_render* push_color( const w_color& color );
	w_render* push_alpha( const float& alpha );
	w_render* push_scale( const float& scale );
	w_render* push_align( const e_align& align );
	w_render* push_depth( const float& depth );
	void end();

	// ----------------------------------------------------------------------------

	bool show_stats = false;

	// circle sample points are stored in a unit circle
	static constexpr int circle_sample_points_max = 16;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	w_render* draw_sprite( a_texture* tex, const w_rect& dst );
	w_render* draw_sprite( const a_subtexture* subtex, const w_rect& dst );

	w_render* draw( a_texture* tex, const w_sz& sz = w_vec2( -1, -1 ) );
	w_render* draw( const a_subtexture* subtex, const w_sz& sz = w_vec2( -1, -1 ) );

	w_render* draw_sliced( const a_9slice_def* slice_def, const w_sz& sz );

	w_render* draw_string( a_font* font, const std::string& text );

	void init_projection() const;
	w_render* draw_rectangle( const w_rect& rc_dst );
	w_render* draw_filled_rectangle( const w_vec2& start, const w_vec2& end );
	w_render* draw_line( const w_vec2& start, const w_vec2& end );
	w_render* draw_circle( const w_vec2& origin, float radius);

	void begin_frame( float frame_interpolate_pct );
	void end_frame();

	w_render* draw_world_axis();
	w_render* draw_stats();

	float calc_interpolated_per_sec_value( float current_value, float step_per_second ) const;
};

