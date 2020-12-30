#pragma once

#include "cache_assets.h"

struct w_pending_collision
{
	w_entity* entity_a;
	b2Fixture* fixture_a;
	w_entity* entity_b;
	b2Fixture* fixture_b;
	b2Manifold manifold;
};

struct w_engine : i_input_receiver
{
	static bool init_game_engine( int argc, char* argv [] );
	static void deinit_game_engine();
	static void exec_main_loop();

#ifndef USE_BASS_SOUND_LIBRARY
	cs_context_t* c2_sound_context = nullptr;
#endif

	a_texture* tex_white = nullptr;
	a_font* pixel_font = nullptr;

	bool is_running = false;
	bool is_paused = false;

	std::unique_ptr<w_opengl_framebuffer> frame_buffer = nullptr;
	std::unique_ptr<w_opengl_framebuffer> blur_frame_buffers[2];
	std::unique_ptr<w_opengl_framebuffer> composite_frame_buffer = nullptr;

	std::unique_ptr<w_physics_debug_draw> physics_debug_draw = nullptr;
	std::unique_ptr<b2World> box2d_world = nullptr;
	std::unique_ptr<w_phys_contact_listener> physics_responder = nullptr;
	std::vector<w_pending_collision> begin_contact_queue;
	std::vector<w_pending_collision> end_contact_queue;

	void new_physics_world();

	virtual ~w_engine() = default;

	void init();
	void deinit();
	void draw();
	void update();
	void toggle_pause();
	void set_pause( bool paused );
	void cache_asset_definition_files( const std::string_view folder_name );
	void precache_asset_resources( int pass, std::string_view game_name );

#if 0 // #texture - wtf are these for?
	a_src_texture* find_texture( const std::string_view name, bool silent = false )
	{
		return find_asset<a_src_texture>( name, silent );
	}
	a_texture* find_subtexture( const std::string_view name, bool silent = false )
	{
		return find_asset<a_texture>( name, silent );
	}
#endif

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
	[[nodiscard]] w_color find_color_from_symbol( const std::string_view str, w_color def_value = w_color::white );
	[[nodiscard]] w_range find_range_from_symbol( const std::string_view str, w_range def_value = w_range( 0, 1 ) );
	[[nodiscard]] w_vec2 find_vec2_from_symbol( const std::string_view str, w_vec2 def_value = w_vec2( 0, 0 ) );

	virtual bool iir_on_released( const w_input_event* evt ) override;

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
};
