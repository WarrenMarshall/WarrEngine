
namespace war
{

struct engine
{
	// the function that all games/apps call to get the engine up and running
	template<typename T>
	static void go( int argc, char* argv [] )
	{
		g_base_game = std::make_unique<T>( T::get_custom_name() );

		launch( argc, argv );
	}

	static void launch( int argc, char* argv [] );
	static void launch_init_engine_assets();
	static void launch_init_frame_buffers();
	static void launch_command_line( int argc, char* argv [] );
	static void launch_precache();
	static void launch_apply_config_settings();

	void main_loop();

	void start_frame();
	void end_frame();

	void do_fixed_time_step();
	void do_draw_frame();
	void do_draw_finished_frame();

	void shutdown();

	// things that need to be updated each frame so the post process shader has
	// all the info it needs
	struct
	{
		float film_grain_amount = 0.0f;
	} post_process;

	std::vector<asset_file_definition> asset_def_file_cache;

	std::deque<std::future<void>> threads;

	texture_asset* tex_white = nullptr;
	font_asset* pixel_font = nullptr;

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

	cmdline_args cmdline;

	std::unique_ptr<opengl_framebuffer> frame_buffer = nullptr;
	std::unique_ptr<opengl_framebuffer> blur_frame_buffer = nullptr;
	std::unique_ptr<opengl_framebuffer> composite_frame_buffer = nullptr;

	struct
	{
		std::unique_ptr<b2World> world = nullptr;
		std::unique_ptr<box2d_physics::debug_draw> debug_draw = nullptr;

		std::unique_ptr<box2d_physics::contact_listener> listener = nullptr;
		std::vector<box2d_physics::pending_collision> begin_contact_queue;
		std::vector<box2d_physics::pending_collision> end_contact_queue;
	} box2d;

	struct
	{
		std::vector<simple_collision::pending_collision> queue;
	} simple_collision;

	render_stats stats;

	void new_physics_world();

	void init();
	void deinit();
	void draw();
	void cache_asset_definition_files( std::string_view folder_name );
	void precache_asset_resources( int pass );
	void wait_for_thread_pool_to_finish();

	void debug_draw_buffers();

	template<typename T>
	[[nodiscard]] T* find_asset( std::string_view name )
	{
		return asset_cache.find<T>( name );
	}

	template<typename T>
	[[nodiscard]] T* find_asset_safe( std::string_view name )
	{
		return asset_cache.find_safe<T>( name );
	}

	void parse_config_files( std::string_view folder_name );
	void parse_config_file( std::string_view filename );

	std::unordered_map<std::string, std::string> _symbol_to_value;
	[[nodiscard]] bool is_symbol_in_map( std::string_view str );
	[[nodiscard]] std::optional<std::string> find_string_from_symbol( std::string_view str );
	[[nodiscard]] int find_int_from_symbol( std::string_view str, int def_value = 0 );
	[[nodiscard]] float find_float_from_symbol( std::string_view str, float def_value = 0.f );
	[[nodiscard]] color find_color_from_symbol( std::string_view str, const color& def_value = color::white );
	[[nodiscard]] range<float> find_range_from_symbol( std::string_view str, const range<float>& def_value = range<float>( 0, 1 ) );
	[[nodiscard]] vec2 find_vec2_from_symbol( std::string_view str, const vec2& def_value = vec2( 0, 0 ) );

	[[nodiscard]] bool on_input_motion( const input_event* evt );
	[[nodiscard]] bool on_input_pressed( const input_event* evt );
	[[nodiscard]] bool on_input_held( const input_event* evt )
	{
		return false;
	}
	[[nodiscard]] bool on_input_released( const input_event* evt );
	[[nodiscard]] bool on_input_key( const input_event* evt )
	{
		return false;
	}

	clock time;
	asset_cache_mgr asset_cache;
	scene_mgr scenes;
	os_window window;
	render renderer;
	input_mgr input;
	opengl render_api;
	key_values config_vars;

	void dispatch_collision_queue();
	void dispatch_simple_collisions();
	void dispatch_box2d_collisions();

	void set_time_dilation( float dilation );

	// message box

	void show_msg_box( std::string_view msg );

	struct
	{
		std::string msg;
	} msg_box;
};

}

