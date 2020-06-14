#pragma once

struct w_accum_value
{
	float accumulator;
	float value;

	void init();
	void update_value( int steps );
	void update_value();
	void accum( float value );
	void inc();
};

// ----------------------------------------------------------------------------

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
	void init();
};

// ----------------------------------------------------------------------------

struct w_render
{
	w_render_stats stats;

	w_color current_clear_color = W_COLOR_DARK_GREY;

	std::stack<glm::mat4> modelview_stack;

	// the current render state
	w_color current_color = W_COLOR_WHITE;
	float current_scale = 1.0f;
	float current_alpha = 1.0f;
	float current_angle = 0.0f;

	bool show_stats = false;

	// circle sample points are stored in a unit circle
	static constexpr int circle_sample_points_max = 32;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	void draw_sprite( w_image* image, const w_sz& sz = w_sz( -1, -1 ), const w_color& color = W_COLOR_WHITE );
	void draw( w_image* image, const w_sz& sz = w_vec2( -1, -1 ), const w_color& color = W_COLOR_WHITE );

	void draw_string( a_font* font, w_vec3 pos, const std::string& text, e_align align, w_color color = W_COLOR_WHITE );
	void draw_sliced_texture( a_texture* texture, const std::string& patch_name, w_rect rc_dst, float z, w_color color = W_COLOR_WHITE );

	void draw_rectangle( w_rect rc_dst );
	void draw_filled_rectangle( w_vec2 start, w_vec2 end, float z, w_color color );
	void draw_line( w_vec3 start, w_vec3 end );
	void draw_circle( w_vec3 origin, float radius);

	void set_clear_color( w_color clear_color );
	void begin();
	void end();

	void draw_world_axis();
	void draw_stats();
};
