
#include "master_pch.h"
#include "master_header.h"

/*
void worker_sample()
{
	while( !engine->exit_thread_sample )
	{
		// do work here

		std::this_thread::sleep_for( 1s );
	}
}
*/

/*
	checks if 'symbol' exists in the map
*/
bool w_engine::is_symbol_in_map( std::string symbol )
{
	return _symbol_to_value.count( symbol ) > 0;
}

/*
	returns a void* to the value stored for 'symbol'
*/
std::string w_engine::find_val_from_symbol( std::string symbol )
{
	if( !is_symbol_in_map( symbol ) )
	{
		log_error( "%s : '%s' not found in symbol table:", __FUNCTION__, symbol.c_str() );
	}

	return _symbol_to_value[symbol];
}

/*
	returns a value to the caller based on the contents of 'symbol'

	if no value is found for 'symbol', the default value is returned
*/
int w_engine::find_int_from_symbol( std::string symbol, int def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		log_msg( "%s : '%s' is not in the symbol map : value defaulted", __FUNCTION__, symbol.c_str() );
		sval = "0";
	}

	return static_cast<int>( strtol( sval.c_str(), (char**) NULL, 10 ) );
}

float w_engine::find_float_from_symbol( std::string symbol, float def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		log_msg( "%s : '%s' is not in the symbol map : value defaulted", __FUNCTION__, symbol.c_str() );
		sval = "0.0";
	}

	return static_cast<float>( strtof( sval.c_str(), (char**) NULL ) );
}

w_color w_engine::find_color_from_symbol( std::string symbol, w_color def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		log_msg( "%s : '%s' is not in the symbol map : value defaulted", __FUNCTION__, symbol.c_str() );
		sval = "1.0,1.0,1.0,1.0";
	}

	return w_color( sval );
}

w_range w_engine::find_range_from_symbol( std::string symbol, w_range def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		log_msg( "%s : '%s' is not in the symbol map : value defaulted", __FUNCTION__, symbol.c_str() );
		sval = "0,1";
	}

	return w_range( sval );
}

w_vec2 w_engine::find_vec2_from_symbol( std::string symbol, w_vec2 def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		log_msg( "%s : '%s' is not in the symbol map : value defaulted", __FUNCTION__, symbol.c_str() );
		sval = "0,0";
	}

	return w_vec2( sval );
}

w_vec3 w_engine::find_vec3_from_symbol( std::string symbol, w_vec3 def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == "n/a" )
	{
		log_msg( "%s : '%s' is not in the symbol map : value defaulted", __FUNCTION__, symbol.c_str() );
		sval = "0,0,0";
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
	input_mgr = std::make_unique<w_input_mgr>();
	fs = std::make_unique<w_file_system>();
	shader = std::make_unique<w_shader>();
	opengl = std::make_unique<w_opengl>();

	input_mgr->add_listener( this );
	w_random::seed();
	fs->init();

	if( g_allow_hot_reload )
	{
		hot_reloadables.clear();
		hot_reloadables_idx = 0;
	}
	//thread_sample = std::thread( worker_sample );
}

void w_engine::deinit()
{
	// wait for threads to finish
	//exit_thread_sample = true;
	//thread_sample.join();

	input_mgr->remove_listener( this );

	opengl->deinit();
}

void w_engine::draw()
{
	engine->layer_mgr->draw();

	// If engine is paused, draw a border around the screen for visibility

	if( is_paused )
	{
		opengl
			->push_identity()
			->translate( w_vec3( -v_window_hw, -v_window_hh, 200 ) );

		render->begin();

		w_vec3 v0, v1, v2, v3;

		v0 = w_vec3::zero;
		v1 = w_vec3( v_window_w - 1, 0, 0 );
		v2 = w_vec3( v_window_w - 1, v_window_h - 1, 0 );
		v3 = w_vec3( 0, v_window_h - 1, 0 );

		render->push_color( W_COLOR_YELLOW );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		v0 = w_vec3( 1, 1, 0 );
		v1 = w_vec3( v_window_w - 2, 1, 0 );
		v2 = w_vec3( v_window_w - 2, v_window_h - 2, 0 );
		v3 = w_vec3( 1, v_window_h - 2, 0 );

		render->push_color( W_COLOR_ORANGE );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		v0 = w_vec3( 2, 2, 0 );
		v1 = w_vec3( v_window_w - 3, 2, 0 );
		v2 = w_vec3( v_window_w - 3, v_window_h - 3, 0 );
		v3 = w_vec3( 2, v_window_h - 3, 0 );

		render->push_color( W_COLOR_RED );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		opengl->pop();
	}

	render->end();
}

void w_engine::update()
{
	layer_mgr->update();

	for( const auto& asset : asset_cache->cache )
	{
		asset.second->update();
	}

	if( g_allow_hot_reload && hot_reloadables.size() )
	{
		hot_reloadables_idx = ( hot_reloadables_idx + 1 ) % static_cast<int>( hot_reloadables.size() );
		i_reloadable* rl = hot_reloadables[hot_reloadables_idx];

		if( rl->needs_reloading() )
		{
			log_msg( "%s : Hot reloading [%s]", __FUNCTION__, rl->original_filename.c_str() );
			rl->clean_up_internals();
			rl->create_internals( true );
		}
	}

	render->show_stats = input_mgr->is_button_down( e_input_id::keyboard_a);
}

void w_engine::toggle_pause()
{
	is_paused = !is_paused;
}

// loads and caches every "*.asset_def" file it sees in the "asset_def" folder

void w_engine::cache_asset_definition_files()
{
	std::vector<std::string> filenames;
	engine->fs->scan_folder_for_ext( filenames, "asset_def", ".asset_def" );

	for( const auto& iter : filenames )
	{
		engine->asset_definition_file_cache->add( iter.c_str(), iter.c_str() );
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
		for( const auto& iter_adf : engine->asset_definition_file_cache->cache )
		{
			iter_adf.second->precache_asset_resources( p, false );
		}
	}

	log_msg( "%s : %d assets precached", __FUNCTION__, engine->asset_cache->cache.size() );
}

void w_engine::on_listener_event_received( e_event_id event, void* object )
{
	w_input_event* evt = static_cast<w_input_event*>( object );

	switch( event )
	{
		case e_event_id::input_motion:
		{
			//RENDER->update_virtual_mouse_pos( w_vec2( evt->pos.x, evt->pos.y ) );
		}
		break;

		case e_event_id::input_pressed:
		{
			switch( evt->data.input_id )
			{
				case e_input_id::keyboard_esc:
				{
					is_running = false;
				}
				break;

				case e_input_id::keyboard_pause:
				{
					toggle_pause();
				}
				break;

				case e_input_id::keyboard_f5:
				{
					render->show_stats = !render->show_stats;
				}
				break;

				case e_input_id::keyboard_f11:
				{
					window->toggle_fullscreen();
				}
				break;

				case e_input_id::keyboard_left_bracket:
				{
					time->dilation -= 0.1f;
					time->dilation = w_max( time->dilation, 0.1f );
				}
				break;

				case e_input_id::keyboard_right_bracket:
				{
					time->dilation += 0.1f;
				}
				break;

				case e_input_id::keyboard_enter:
				{
					bool alt_down = ( evt->data.mods & GLFW_MOD_ALT );

					if( alt_down )
					{
						window->toggle_fullscreen();
					}
				}
				break;

				case e_input_id::keyboard_f10:
				{
					log_msg( " " );
					log_msg( "-- RENDER STATS -----------------" );
					log_msg( " " );
					log_msg( "\tRender Buffers:" );

					w_render_buffer* rb;

					for( const auto& iter : asset_cache->cache )
					{
						if( iter.second->render_buffer )
						{
							rb = iter.second->render_buffer.get();
							rb->log_stats( iter.second.get() );
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
