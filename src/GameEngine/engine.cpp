
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

#if !defined( FINALRELEASE )
void tw_refresh_reloadables()
{
	log_msg( fmt::format( "Worker thread starting : {}", __FUNCTION__ ) );

	while( !engine->exit_tw_refresh_reloadables )
	{
		for( auto& iter : engine->hot_reloadables )
		{
			{
				std::scoped_lock lock( iter->mutex_last_write_time );
				iter->last_write_time_on_disk = iter->retrieve_last_write_time_from_disk();
			}

			std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
		}
	}

	log_msg( fmt::format( "Worker thread exiting : {}", __FUNCTION__ ) );
}
#endif

// ----------------------------------------------------------------------------

/*
	checks if 'symbol' exists in the map
*/
bool w_engine::is_symbol_in_map( const std::string_view symbol )
{
	return _symbol_to_value.count( std::string( symbol ) ) > 0;
}

/*
	returns a void* to the value stored for 'symbol'
*/
std::string_view w_engine::find_val_from_symbol( std::string_view symbol )
{
	if( !is_symbol_in_map( symbol ) )
	{
		return "n/a";
	}

	return _symbol_to_value[ std::string( symbol ) ];
}

/*
	returns a value to the caller based on the contents of 'symbol'

	if no value is found for 'symbol', the default value is returned
*/
int w_engine::find_int_from_symbol( std::string_view symbol, int def_value )
{
	std::string_view sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<int>( strtol( sval.data(), (char**) nullptr, 10 ) );
}

float w_engine::find_float_from_symbol( std::string_view symbol, float def_value )
{
	std::string_view sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<float>( strtof( sval.data(), (char**) nullptr ) );
}

w_color w_engine::find_color_from_symbol( std::string_view symbol, w_color def_value )
{
	std::string_view sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		sval = fmt::format( "{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a );
	}

	return w_color( sval );
}

w_range w_engine::find_range_from_symbol( std::string_view symbol, w_range def_value )
{
	std::string_view sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		sval = fmt::format( "{},{}", def_value.min, def_value.max );
	}

	return w_range( sval );
}

w_vec2 w_engine::find_vec2_from_symbol( std::string_view symbol, w_vec2 def_value )
{
	std::string_view sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		sval = fmt::format( "{},{}", def_value.x, def_value.y );
	}

	return w_vec2( sval );
}

w_vec3 w_engine::find_vec3_from_symbol( std::string_view symbol, w_vec3 def_value )
{
	std::string_view sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		sval = fmt::format( "{},{},{}", def_value.x, def_value.y, def_value.z );
	}

	return w_vec3( sval );
}

/*
	called ONCE, as the engine is starting up
*/

void w_engine::init()
{
	// make sure we are only calling this function ONE time per instance
	assert( time == nullptr );

	time = std::make_unique<w_time>();
	asset_definition_file_cache = std::make_unique<w_cache_asset_definition_files>();
	asset_cache = std::make_unique<w_cache_assets>();
	layer_mgr = std::make_unique<w_layer_mgr>();
	window = std::make_unique<w_window>();
	render = std::make_unique<w_render>();
	input = std::make_unique<w_input>();
	ui = std::make_unique<w_ui_mgr>();
	fs = std::make_unique<w_file_system>();
	shader = std::make_unique<w_shader>();
	opengl = std::make_unique<w_opengl>();

	input->add_listener( this );
	input->add_listener( layer_mgr.get() );
	
	w_random::seed();
	fs->init();

	hot_reload_timer = std::make_unique<w_timer>( 1000 );

	if( g_allow_hot_reload )
	{
		hot_reloadables.clear();
	}
}

void w_engine::deinit()
{
	input->remove_listener( this );
	input->remove_listener( layer_mgr.get() );

	// wait for threads to finish
#if !defined( FINALRELEASE )
	exit_tw_refresh_reloadables = true;
	t_refresh_reloadables.join();
#endif
}

void w_engine::draw()
{
	// If engine is paused, draw a border around the screen for visibility

	if( is_paused )
	{
		render
			->begin()
			->push_depth( zdepth_engine );

		w_vec2 v0, v1, v2, v3;

		v0 = w_vec2::zero;
		v1 = w_vec2( v_window_w - 1, 0.0f );
		v2 = w_vec2( v_window_w - 1, v_window_h - 1 );
		v3 = w_vec2( 0.0f, v_window_h - 1 );

		render->push_rgb( W_COLOR_YELLOW );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		v0 = w_vec2( 1, 1 );
		v1 = w_vec2( v_window_w - 2, 1.0f );
		v2 = w_vec2( v_window_w - 2, v_window_h - 2 );
		v3 = w_vec2( 1.0f, v_window_h - 2 );

		render->push_rgb( W_COLOR_ORANGE );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		v0 = w_vec2( 2.0f, 2.0f );
		v1 = w_vec2( v_window_w - 3, 2.0f );
		v2 = w_vec2( v_window_w - 3, v_window_h - 3 );
		v3 = w_vec2( 2.0f, v_window_h - 3 );

		render->push_rgb( W_COLOR_RED );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		render->end();
	}
}

void w_engine::update()
{
	layer_mgr->update();

	for( const auto& [xxx, asset] : asset_cache->cache )
	{
		asset->update();
	}

	render->show_stats = input->is_button_down( input_id::key_a);
}

// each time this is called, it checks the reload status for a single i_reloadable

void w_engine::update_hot_reload()
{
	if( !g_allow_hot_reload )
	{
		return;
	}

	for( auto& iter : hot_reloadables )
	{
		if( iter->needs_reloading() )
		{
			std::scoped_lock lock( iter->mutex_last_write_time );

			log_msg( fmt::format( "{} : Hot reloading [{}]", __FUNCTION__, iter->original_filename ) );
			iter->clean_up_internals();
			iter->create_internals( b_is_hot_reloading( true ) );
			iter->last_write_time = iter->last_write_time_on_disk;
		}
	}

}

void w_engine::toggle_pause()
{
	is_paused = !is_paused;
}

void w_engine::set_pause( bool paused )
{
	is_paused = paused;
}

// loads and caches every "*.asset_def" file it sees in the "asset_def" folder

void w_engine::cache_asset_definition_files()
{
	std::vector<std::string> filenames;
	engine->fs->scan_folder_for_ext( filenames, "asset_def", ".asset_def" );

	for( const auto& iter : filenames )
	{
		engine->asset_definition_file_cache->add( iter, iter );
	}
}

/*
	iterate through all the cached asset_definition_files and look
	for any asset_definitions that have assets we need to precache

	things like texture files, sound files, etc.
*/
void w_engine::precache_asset_resources()
{
	for( int p = 0; p < w_engine::num_asset_def_passes; ++p )
	{
		for( const auto& [xxx, asset_definition_file] : engine->asset_definition_file_cache->cache )
		{
			asset_definition_file->precache_asset_resources( p, b_is_hot_reloading(false) );
		}
	}

	log_msg( fmt::format( "{} : {} assets precached", __FUNCTION__, s_commas( static_cast<float>( engine->asset_cache->cache.size() ) ) ) );

	// start the thread that monitors the reloadables for changes
#if !defined( FINALRELEASE )
	t_refresh_reloadables = std::thread( tw_refresh_reloadables );
#endif
}

void w_engine::on_listener_event_received( e_event_id event, void* object )
{
	auto evt = static_cast<w_input_event*>( object );

	switch( event )
	{
		case event_id::input_pressed:
		{
			switch( evt->input_id )
			{
				case input_id::key_esc:
				{
					is_running = false;
				}
				break;

				case input_id::key_pause:
				{
					toggle_pause();
				}
				break;

				case input_id::key_f11:
				{
					window->toggle_fullscreen();
				}
				break;

				case input_id::key_left_bracket:
				{
					time->dilation -= 0.1f;
					time->dilation = w_max( time->dilation, 0.1f );
				}
				break;

				case input_id::key_right_bracket:
				{
					time->dilation += 0.1f;
				}
				break;

				case input_id::key_enter:
				{
					if( engine->input->is_alt_down() )
					{
						window->toggle_fullscreen();
					}
				}
				break;

				case input_id::key_f10:
				{
					log_msg( " " );
					log_msg( "-- RENDER STATS -----------------" );
					log_msg( " " );
					log_msg( "\tRender Buffers:" );

					w_render_buffer* rb;

					for( const auto& [xxx, asset] : asset_cache->cache )
					{
						if( asset->render_buffer )
						{
							rb = asset->render_buffer.get();
							rb->log_stats( asset.get() );
						}
					}

					log_msg( " " );

					engine->toggle_pause();
				}
				break;
			}
		}
		break;
	}
}
