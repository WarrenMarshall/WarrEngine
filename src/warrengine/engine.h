
namespace war
{

struct Engine final
{
	// the function that all games/apps call to get the engine up and running
	template<typename T>
	static void go( i32 argc, char* argv [] )
	{
		g_base_game = std::make_unique<T>();

		launch( argc, argv );
	}

	static void launch( i32 argc, char* argv [] );
	static void parse_command_line( i32 argc, char* argv [] );
	static void precache();
	static void apply_config_settings();

	void main_loop();

	void do_draw_finished_frame();

	void shutdown();

	// things that need to be updated each frame so the post process shader has
	// all the info it needs
	struct
	{
		f32 film_grain_amount = 0.f;
	} post_process;

	std::vector<Asset_File_Definition> asset_def_file_cache;

	std::vector<std::future<void>> threads;

	Texture_Asset* tex_white = nullptr;
	Font_Asset* pixel_font = nullptr;

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

	Cmdline_Args cmdline;

	OpenGL_Frame_Buffer frame_buffer;
	OpenGL_Frame_Buffer blur_frame_buffer;
	OpenGL_Frame_Buffer composite_frame_buffer;

	struct
	{
		std::unique_ptr<b2World> world = nullptr;
		box2d::Box2D_Debug_Draw debug_draw;
		box2d::Contact_Listener listener;
		std::vector<box2d::Pending_Collision> begin_contact_queue;
		std::vector<box2d::Pending_Collision> end_contact_queue;
	} box2d;

	Render_Stats stats;

	void new_physics_world();

	void init();
	void deinit();
	void draw();
	void cache_asset_definition_files( std::string_view folder_name );
	void precache_asset_resources( i32 pass );
	void wait_for_thread_pool_to_finish();

	void debug_draw_buffers();

	template<typename T>
	[[nodiscard]] T* find_asset( const std::string& name )
	{
		return asset_cache.find<T>( name );
	}

	template<typename T>
	[[nodiscard]] T* find_asset_safe( const std::string& name )
	{
		return asset_cache.find_safe<T>( name );
	}

	void parse_config_files( std::string_view folder_name );
	void parse_config_file( std::string_view filename );

	std::unordered_map<std::string, std::string> _symbol_to_value;
	[[nodiscard]] bool is_symbol_in_map( std::string_view str );
	[[nodiscard]] std::optional<std::string> find_string_from_symbol( std::string_view str );
	[[nodiscard]] bool find_bool_from_symbol( std::string_view str, bool def_value = true );
	[[nodiscard]] i32 find_int_from_symbol( std::string_view str, i32 def_value = 0 );
	[[nodiscard]] f32 find_float_from_symbol( std::string_view str, f32 def_value = 0.f );
	[[nodiscard]] Color find_color_from_symbol( std::string_view str, const Color& def_value = Color::white );
	[[nodiscard]] Range find_range_from_symbol( std::string_view str, const Range& def_value = Range( 0, 1 ) );
	[[nodiscard]] Vec2 find_vec2_from_symbol( std::string_view str, const Vec2& def_value = Vec2( 0, 0 ) );

	[[nodiscard]] bool on_input( const Input_Event* evt );

	Clock clock;
	Asset_Cache_Mgr asset_cache;
	Scene_Mgr scene_mgr;
	OS_Window window;
	Render render;
	Input_Mgr input_mgr;
	OpenGL_Mgr opengl_mgr;
	Key_Values config_vars;

	void dispatch_collision_queue();
	void dispatch_box2d_collisions();

	void set_time_dilation( f32 dilation );

	// message box

	void show_msg_box( std::string_view msg );

	struct
	{
		std::string msg;
	} msg_box;
};

}

