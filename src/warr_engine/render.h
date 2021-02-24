#pragma once

// ----------------------------------------------------------------------------

struct w_render_state
{
	e_align align = align::left;
	float angle = 0.0f;
	w_color color = w_color::white;
	float glow = 0.0f;
	float pick_id = 0.0f;
	w_vec2 scale = { 1.0f, 1.0f };
	bool snap_to_pixel = true;
	w_vec2 uv_tiling = { 1.0f, 1.0f };
	float z = 0.0f;

	void set_from_opt( w_render_state_opt& rso );
};

struct w_render_state_opt
{
	std::optional<e_align> align = std::nullopt;
	std::optional<float> angle = std::nullopt;
	std::optional<w_color> color = std::nullopt;
	std::optional<float> glow = std::nullopt;
	std::optional<float> pick_id = std::nullopt;
	std::optional<w_vec2> scale = std::nullopt;
	std::optional<bool> snap_to_pixel = std::nullopt;
	std::optional<w_vec2> uv_tiling = std::nullopt;
	std::optional<float> z = std::nullopt;
};

// making sure that these are small enough to fit into the cache nicely and
// that they are fast to copy around.

static_assert( sizeof( w_render_state ) <= 64 );
static_assert( std::is_trivially_assignable<w_render_state, w_render_state>() );
static_assert( std::is_trivially_copy_constructible<w_render_state>() );
static_assert( std::is_trivially_copyable<w_render_state>() );

// ----------------------------------------------------------------------------

struct w_render
{
	a_palette* palette = nullptr;

	w_color get_palette_color_from_idx( int idx );

	bool batches_are_empty();

	// a value from 0.0-1.0 that represents how near we are to the NEXT update
	// tick. this is used to interpolate/predict rendering for smooth movement
	// and rotations even at low FTS values.

	float frame_interpolate_pct = 0.0f;

	// batch rendering
	std::unique_ptr<w_render_batch> batch_lines = nullptr;
	std::unique_ptr<w_render_batch> batch_points = nullptr;
	std::unique_ptr<w_render_batch> batch_quads = nullptr;
	std::unique_ptr<w_render_batch> batch_triangles = nullptr;

	void draw_and_reset_all_batches();

	// palette support
	a_palette* current_palette = nullptr;
	w_color pal_color_from_idx( size_t idx );

	void clear_render_state_stack();

	bool single_frame_debugger = false;
	bool show_extra_debug = true;

	// ----------------------------------------------------------------------------

	bool show_stats = false;

	// circle sample points are stored in a unit circle
	constexpr static int circle_sample_points_max = 16;
	w_vec2 circle_sample_points[circle_sample_points_max];

	void init();

	static void draw_quad( const a_texture* texture, const w_rect& dst );
	static void draw_circle( const w_vec2& origin, const float radius );
	static void draw_filled_circle( const w_vec2& origin, const float radius );
	static void draw_filled_rectangle( const w_rect& dst );
	static void draw_filled_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 );
	static void draw_line( const w_vec2& start, const w_vec2& end );
	static void draw_mesh( a_mesh* mesh );
	static void draw_point( const w_vec2& pos );
	static void draw_rectangle( const w_rect& dst );
	static void draw_sliced( const a_9slice_def* slice_def, const w_rect& dst );
	static void draw_sprite( const a_texture* texture, const w_vec2& dst );
	static void draw_stats();
	static void draw_string( const a_font* font, const std::string_view text, const w_pos& pos );
	static void draw_string( const std::string_view text, const w_pos& pos );
	static void draw_tiled( const a_texture* texture, const w_rect& dst );
	static void draw_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 );
	static void draw_world_axis();

	void begin_frame() const;
	void end_frame();

	[[nodiscard]] float calc_interpolated_per_sec_value( float current_value, float step_per_second ) const;

	// ----------------------------------------------------------------------------

	std::vector<w_render_state> render_states;
	w_render_state* top_render_state = nullptr;

	w_render* push();
	w_render* pop();

	[[nodiscard]] static int sample_pick_id_at( w_vec2 click_pos );
};
