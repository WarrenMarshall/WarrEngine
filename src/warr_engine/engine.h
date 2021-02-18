#pragma once

#include "cache_assets.h"

// ----------------------------------------------------------------------------

struct w_pending_collision
{
	w_entity* entity_a;
	b2Fixture* fixture_a;
	w_entity* entity_b;
	b2Fixture* fixture_b;
	b2Manifold manifold;
	b2WorldManifold wmanifold;
};

// ----------------------------------------------------------------------------

struct w_cmdline_args
{
	// "-verbose" to set TRUE
	//		* enables optional log messages that are marked using "log_verbose"
	bool verbose = false;

	// "-nobatch" to set TRUE
	//		* pretend like the video card has a single texture unit
	bool nobatch = false;
};

// ----------------------------------------------------------------------------

struct w_engine
{
	// the function that all games/apps call to get the engine up and running
	template<typename T>
	static void go( int argc, char* argv [] )
	{
		base_game = std::make_unique<T>( T::get_custom_name() );

		launch( argc, argv );
	}

	static void launch( int argc, char* argv [] );
	void main_loop();
	void shutdown();

	std::vector<std::future<void>> threads;

	a_texture* tex_white = nullptr;
	a_font* pixel_font = nullptr;

	bool is_running = false;

	struct
	{
		// if pause_toggle is true or pause_ref_count > 0, the engine is paused
		bool toggle = false;
		int16 ref_count = 0;
	} pause_state;

	void toggle_pause();
	void pause();
	void resume();
	bool is_paused();

	w_cmdline_args cmdline;

	std::unique_ptr<w_opengl_framebuffer> frame_buffer = nullptr;
	std::unique_ptr<w_opengl_framebuffer> blur_frame_buffers[ 2 ] = { nullptr, nullptr };
	std::unique_ptr<w_opengl_framebuffer> composite_frame_buffer = nullptr;

	std::unique_ptr<w_physics_debug_draw> physics_debug_draw = nullptr;
	std::unique_ptr<b2World> box2d_world = nullptr;
	std::unique_ptr<w_physics_contact_listener> physics_responder = nullptr;
	std::vector<w_pending_collision> begin_contact_queue;
	std::vector<w_pending_collision> end_contact_queue;

	std::unique_ptr<w_render_stats> stats = nullptr;

	void new_physics_world();

	virtual ~w_engine() = default;

	void init();
	void deinit();
	void draw();
	void update();
	void cache_asset_definition_files( const std::string_view folder_name );
	void precache_asset_resources( int pass );
	void wait_for_thread_pool_to_finish();

	template<typename T>
	T* find_asset( const std::string_view name, bool silent = false )
	{
		return asset_cache->find<T>( name, silent );
	}

	void parse_config_files( const std::string_view folder_name );
	void parse_config_file( std::string_view filename );

	std::unordered_map<std::string, std::string> _symbol_to_value;
	[[nodiscard]] bool is_symbol_in_map( const std::string_view str );
	[[nodiscard]] std::optional<std::string> find_string_from_symbol( const std::string_view str );
	[[nodiscard]] int find_int_from_symbol( const std::string_view str, int def_value = 0 );
	[[nodiscard]] float find_float_from_symbol( const std::string_view str, float def_value = 0.0f );
	[[nodiscard]] w_color find_color_from_symbol( const std::string_view str, const w_color& def_value = w_color::white );
	[[nodiscard]] w_range find_range_from_symbol( const std::string_view str, const w_range& def_value = w_range( 0, 1 ) );
	[[nodiscard]] w_vec2 find_vec2_from_symbol( const std::string_view str, const w_vec2& def_value = w_vec2( 0, 0 ) );

	bool on_input_motion( const w_input_event* evt ) { return false; }
	bool on_input_pressed( const w_input_event* evt );
	bool on_input_held( const w_input_event* evt ) { return false; }
	bool on_input_released( const w_input_event* evt );
	bool on_input_key( const w_input_event* evt ) { return false; }

	std::unique_ptr<w_cache_asset_definition_files> asset_definition_file_cache = nullptr;
	std::unique_ptr<w_time> time = nullptr;
	std::unique_ptr<w_cache_assets> asset_cache = nullptr;
	std::unique_ptr<w_layer_mgr> layer_mgr = nullptr;
	std::unique_ptr<w_window> window = nullptr;
	std::unique_ptr<w_render> render = nullptr;
	std::unique_ptr<w_input> input = nullptr;
	std::unique_ptr<w_ui_mgr> ui = nullptr;
	std::unique_ptr<w_file_system> fs = nullptr;
	std::unique_ptr<w_opengl> opengl = nullptr;
	std::unique_ptr<w_keyvalues> config_vars = nullptr;
	std::unique_ptr<w_random> random = nullptr;

	void process_collision_queue();

	void set_time_dilation( float dilation );
	void adjust_assets_for_time_dilation();
};
