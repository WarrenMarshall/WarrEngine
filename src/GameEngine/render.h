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
	float frame_interpolate_pct;

	std::stack<glm::mat4> modelview_stack;

	// ----------------------------------------------------------------------------
	// the current render state

	bool rs_color_used = false;
	std::stack<w_color> rs_color_stack;

	w_render* begin();
	w_render* rs_color( const w_color& color );
	void end();

	// ----------------------------------------------------------------------------

	bool show_stats = false;

	// circle sample points are stored in a unit circle
	static constexpr int circle_sample_points_max = 32;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	w_render* draw_sprite( a_image* image, const w_sz& sz = w_sz( -1, -1 ) );
	w_render* draw( a_image* image, const w_sz& sz = w_vec2( -1, -1 ) );
	w_render* draw_string( a_font* font, w_vec3 pos, const std::string& text, e_align align );
	w_render* draw_sliced_texture( a_texture* texture, const std::string& patch_name, w_rect rc_dst, float z );

	w_render* draw_rectangle( w_rect rc_dst );
	w_render* draw_filled_rectangle( w_vec2 start, w_vec2 end, float z );
	w_render* draw_line( w_vec3 start, w_vec3 end );
	w_render* draw_circle( w_vec3 origin, float radius);

	void begin_frame( float frame_interpolate_pct );
	void end_frame();

	w_render* draw_world_axis();
	w_render* draw_stats();
};

