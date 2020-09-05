#pragma once

#include "cache_assets.h"

/*
	holds the game engine state
*/

struct w_engine : i_listener
{
	static bool init_game_engine( std::string_view game_name, int argc, char* argv [], w_game* custom_game );
	static void deinit_game_engine();
	static void exec_main_loop();

	a_subtexture* white_wire = nullptr;
	a_subtexture* white_solid = nullptr;
	a_font* pixel_font = nullptr;

	bool is_running = false;
	bool is_paused = false;

	virtual ~w_engine() = default;

	void init();
	void deinit();
	void draw();
	void update();
	void toggle_pause();
	void set_pause( bool paused );
	void cache_asset_definition_files( const std::string_view folder_name );
	void precache_asset_resources( int pass, std::string_view game_name );
	template<typename T> [[nodiscard]] T* get_asset( const std::string_view name, bool silent = false )
	{
		return static_cast<T*>( asset_cache->find( name, silent ) );
	}
	void parse_config_files( const std::string_view folder_name );
	void parse_config_file( std::string_view filename );

	std::map<std::string, std::string> _symbol_to_value;
	bool is_symbol_in_map( const std::string_view str );
	std::string find_string_from_symbol( const std::string_view str );
	int find_int_from_symbol( const std::string_view str, int def_value = 0 );
	float find_float_from_symbol( const std::string_view str, float def_value = 0.0f );
	w_color find_color_from_symbol( const std::string_view str, w_color def_value = w_color::white );
	w_range find_range_from_symbol( const std::string_view str, w_range def_value = w_range( 0, 1 ) );
	w_vec2 find_vec2_from_symbol( const std::string_view str, w_vec2 def_value = w_vec2( 0, 0 ) );
	w_vec3 find_vec3_from_symbol( const std::string_view str, w_vec3 def_value = w_vec3( 0, 0, 0 ) );

	std::unique_ptr<w_cache_asset_definition_files> asset_definition_file_cache = nullptr;

	std::unique_ptr<w_time> time = nullptr;
	std::unique_ptr<w_cache_assets> asset_cache = nullptr;
	std::unique_ptr<w_layer_mgr> layer_mgr = nullptr;
	std::unique_ptr<w_window> window = nullptr;
	std::unique_ptr<w_render> render = nullptr;
	std::unique_ptr<w_input> input = nullptr;
	std::unique_ptr<w_ui_mgr> ui = nullptr;
	std::unique_ptr<w_file_system> fs = nullptr;
	std::unique_ptr<w_shader> shader = nullptr;
	std::unique_ptr<w_opengl> opengl = nullptr;
	std::unique_ptr<w_keyvalues> config_vars = nullptr;

	void on_listener_event_received( e_event_id event, void* object ) override;
};
