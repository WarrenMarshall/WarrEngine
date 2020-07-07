#pragma once

#include "cache_assets.h"

/*
	holds the game engine state
*/

struct w_engine : i_listener
{
	// these are only used if "+hot_reload" is passed into the command line
	std::vector<i_reloadable*> hot_reloadables;
	std::unique_ptr<w_timer> hot_reload_timer = nullptr;

	a_subtexture* white_wire = nullptr;
	a_subtexture* white_solid = nullptr;

	bool is_running = false;
	bool is_paused = false;

	static constexpr int num_asset_def_passes = 3;

	void init();
	void deinit();
	void draw();
	void update();
	void update_hot_reload();
	void toggle_pause();
	void set_pause( bool paused );
	void cache_asset_definition_files();
	void precache_asset_resources();
	template<typename T> T* get_asset( const std::string& name, bool silent = false )
	{
#if !defined(FINALRELEASE)
		return dynamic_cast<T*>( asset_cache->find( name, silent ) );
#else
		return static_cast<T*>( asset_cache->find( name, silent ) );
#endif
	}

	std::map<std::string, std::string> _symbol_to_value;
	bool is_symbol_in_map( std::string str );
	std::string find_val_from_symbol( std::string str );
	int find_int_from_symbol( std::string str, int def_value = 0 );
	float find_float_from_symbol( std::string str, float def_value = 0.0f );
	w_color find_color_from_symbol( std::string str, w_color def_value = W_COLOR_WHITE );
	w_range find_range_from_symbol( std::string str, w_range def_value = w_range( 0, 1 ) );
	w_vec2 find_vec2_from_symbol( std::string str, w_vec2 def_value = w_vec2( 0, 0 ) );
	w_vec3 find_vec3_from_symbol( std::string str, w_vec3 def_value = w_vec3( 0, 0, 0 ) );

	std::unique_ptr<w_cache_asset_definition_files> asset_definition_file_cache = nullptr;

	std::unique_ptr<w_time> time = nullptr;
	std::unique_ptr<w_cache_assets> asset_cache = nullptr;
	std::unique_ptr<w_layer_mgr> layer_mgr = nullptr;
	std::unique_ptr<w_window> window = nullptr;
	std::unique_ptr<w_render> render = nullptr;
	std::unique_ptr<w_input_mgr> input_mgr = nullptr;
	std::unique_ptr<w_ui_mgr> ui_mgr = nullptr;
	std::unique_ptr<w_file_system> fs = nullptr;
	std::unique_ptr<w_shader> shader = nullptr;
	std::unique_ptr<w_opengl> opengl = nullptr;

	std::thread t_refresh_reloadables;
	std::atomic_bool exit_thread_sample = false;
	
	void on_listener_event_received( e_event_id event, void* object ) override;
};
