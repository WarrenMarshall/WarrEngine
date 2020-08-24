
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

bool w_engine::init_game_engine( std::string_view game_name, int argc, char* argv [], w_game* custom_game )
{
	try
	{

#if defined(FINALRELEASE)
		ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

		// get the log file running so we can immediately start writing into it
		logfile = std::make_unique<w_logfile>();
		logfile->init( game_name );

		// create the game engine
		engine = std::make_unique<w_engine>();

		{	// starting
			logfile->time_stamp( "Started" );
		}

		{	// engine
			log_msg( "Initializing engine" );
			engine->init();
		}

		// #todo : write a proper command line parsing class
		{	// command line parsing
		}

		{	// window
			log_msg( "Creating window" );
			if( !engine->window->init() )
			{
				return false;
			}
		}

		{	// opengl
			log_msg( "Initializing OpenGL" );
			OPENGL->init();
		}

		{	// renderer
			log_msg( "Initializing renderer" );
			RENDER->init();
		}

		{	// audio
			log_msg( "Initializing BASS audio" );
			if( !BASS_Init( -1, 44100, 0, nullptr, nullptr ) )
			{
				log_warning( "BASS Audio : init failed!" );
			}
		}

		{
			// read asset definitions and cache them
			log_msg( "Caching asset definition files (*.asset_def)..." );
			engine->cache_asset_definition_files( "game_engine_data" );
			engine->cache_asset_definition_files( fmt::format( "{}_data", game_name ) );

			// look through the cached asset definitions and load any assets required
			// into the asset cache
			log_msg( "Precaching resources from definition files..." );
			engine->precache_asset_resources();

			// now that the preproc files are loaded, parse out the fields specific
			// to the window and apply them as needed.

			v_window_w = engine->find_float_from_symbol( "v_window_w", 100 );
			v_window_h = engine->find_float_from_symbol( "v_window_h", 100 );

			w_rect rc = engine->window->compute_max_window_size_for_desktop();
			glfwSetWindowPos( engine->window->window, static_cast<int>( rc.x ), static_cast<int>( rc.y ) );
			glfwSetWindowSize( engine->window->window, static_cast<int>( rc.w ), static_cast<int>( rc.h ) );
			glfwSetWindowAspectRatio( engine->window->window,
									  100,
									  static_cast<int>( ( v_window_h / v_window_w ) * 100 ) );

			glfwSwapInterval( bool( engine->find_val_from_symbol( "v_sync" ) == "true" ) );
			engine->window->set_title( engine->find_val_from_symbol( "app_title" ) );
			glfwSetWindowAttrib( engine->window->window, GLFW_FLOATING, bool( engine->find_val_from_symbol( "always_on_top" ) == "true" ) );
		}

		// game

		log_msg( "Initializing game" );
		game = custom_game;
		game->init();
		game->new_game();

		// input initialization

		log_msg( "Initializing input" );
		engine->input->init();

		engine->is_running = true;
		engine->time->init();

		// initialize random internals
		{
			// used for wireframe drawing
			engine->white_wire = engine->get_asset<a_subtexture>( "engine_white_wire" );
			engine->white_wire->tex->render_buffer->prim_type = GL_LINES;

			// used for solid drawing
			engine->white_solid = engine->get_asset<a_subtexture>( "engine_white_solid" );

			engine->ui->init();
			engine->ui->theme->init();

			// using a custom mouse cursor, so hide the system mouse
			engine->window->set_mouse_mode( mouse_mode::hidden );
		}
	}
	catch( std::exception& e )
	{
		log_msg( "!! EXCEPTION CAUGHT !!" );
		log_msg( fmt::format( "\t{}", e.what() ) );

		MessageBoxA( nullptr, e.what(), "Exception!", MB_OK );
	}

	return true;
}

void w_engine::deinit_game_engine()
{
	// Clean up

	log_msg( "Shutting down..." );

	log_msg( "Shutting down window" );
	engine->window->deinit();

	log_msg( "Shutting down OpenGL" );
	glDeleteProgram( engine->shader->id );

	log_msg( "Shutting down GLFW" );
	glfwTerminate();

	log_msg( "Shutting down BASS Audio" );
	BASS_Free();

	log_msg( "Shutting down input" );
	engine->input->deinit();

	log_msg( "Shutting down engine" );
	engine->deinit();

	// Do this last so we can log right up until the last moment
	logfile->time_stamp( "Ended" );
	log_msg( "Finished!" );
	logfile->deinit();
}

void w_engine::exec_main_loop()
{
	/*
		main game loop
	*/

	while( engine->is_running )
	{
		if( glfwWindowShouldClose( engine->window->window ) )
		{
			engine->is_running = false;
		}

		/*
			event processing
		*/
		glfwWaitEventsTimeout( 0.001 );

		/*
			update core engine stuff - time, timers, etc
		*/

		engine->time->update();

		/*
			process user input
		*/

		engine->input->update();
		UI->im_reset();

		/*
			if we have fixed time steps to perform, walk
			through them one at a time
		*/

		while( engine->time->fts_accum_ms >= w_time::FTS_step_value_ms )
		{
			engine->time->fts_accum_ms -= w_time::FTS_step_value_ms;

			engine->update();
			game->update();
		}

		/*
			draw everything
		*/

		RENDER->init_projection();

		// whatever remaining ms are left in engine->time->fts_accum_ms should be passed
		// to the render functions for interpolation/prediction
		//
		// it is passed a percentage for easier use : 0.0f-1.0f

		RENDER->begin_frame( engine->time->fts_accum_ms / w_time::FTS_step_value_ms );
		{
			engine->layer_mgr->draw();

			UI->draw_topmost();
			engine->draw();
		}
		RENDER->end_frame();
	}
}

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
std::string w_engine::find_val_from_symbol( std::string_view symbol )
{
	if( !is_symbol_in_map( symbol ) )
	{
		return str_not_found;
	}

	return _symbol_to_value[ std::string( symbol ) ];
}

/*
	returns a value to the caller based on the contents of 'symbol'

	if no value is found for 'symbol', the default value is returned
*/
int w_engine::find_int_from_symbol( std::string_view symbol, int def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<int>( strtol( sval.data(), ( char** ) nullptr, 10 ) );
}

float w_engine::find_float_from_symbol( std::string_view symbol, float def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<float>( strtof( sval.data(), ( char** ) nullptr ) );
}

w_color w_engine::find_color_from_symbol( std::string_view symbol, w_color def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a );
	}

	return w_color( sval );
}

w_range w_engine::find_range_from_symbol( std::string_view symbol, w_range def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{},{}", def_value.min, def_value.max );
	}

	return w_range( sval );
}

w_vec2 w_engine::find_vec2_from_symbol( std::string_view symbol, w_vec2 def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{},{}", def_value.x, def_value.y );
	}

	return w_vec2( sval );
}

w_vec3 w_engine::find_vec3_from_symbol( std::string_view symbol, w_vec3 def_value )
{
	std::string sval = find_val_from_symbol( symbol );

	if( sval == str_not_found )
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

	stbi_set_flip_vertically_on_load( 1 );

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
}

void w_engine::deinit()
{
	input->remove_listener( this );
	input->remove_listener( layer_mgr.get() );
}

void w_engine::draw()
{
	// If engine is paused, draw a border around the screen for visibility

	if( is_paused )
	{
		/*
		render
			->begin()
			->push_depth( zdepth_engine );

		w_vec2 v0, v1, v2, v3;

		v0 = w_vec2::zero;
		v1 = w_vec2( v_window_w - 1, 0.0f );
		v2 = w_vec2( v_window_w - 1, v_window_h - 1 );
		v3 = w_vec2( 0.0f, v_window_h - 1 );

		render->push_rgb( w_color::yellow );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		v0 = w_vec2( 1, 1 );
		v1 = w_vec2( v_window_w - 2, 1.0f );
		v2 = w_vec2( v_window_w - 2, v_window_h - 2 );
		v3 = w_vec2( 1.0f, v_window_h - 2 );

		render->push_rgb( w_color::orange );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		v0 = w_vec2( 2.0f, 2.0f );
		v1 = w_vec2( v_window_w - 3, 2.0f );
		v2 = w_vec2( v_window_w - 3, v_window_h - 3 );
		v3 = w_vec2( 2.0f, v_window_h - 3 );

		render->push_rgb( w_color::red );
		render->draw_line( v0, v1 );
		render->draw_line( v1, v2 );
		render->draw_line( v2, v3 );
		render->draw_line( v3, v0 );

		render->end();
		*/
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

void w_engine::toggle_pause()
{
	is_paused = !is_paused;
}

void w_engine::set_pause( bool paused )
{
	is_paused = paused;
}

// loads and caches every "*.asset_def" file it sees in the "asset_def" folder

void w_engine::cache_asset_definition_files( const std::string_view folder_name )
{
	std::vector<std::string> filenames;
	engine->fs->scan_folder_for_ext( filenames, fmt::format( "{}", folder_name ), ".asset_def" );

	for( const auto& iter : filenames )
	{
		engine->asset_definition_file_cache->add( iter );
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
		for( const auto& asset_definition_file : engine->asset_definition_file_cache->cache )
		{
			asset_definition_file->precache_asset_resources( p );
		}
	}

	log_msg( fmt::format( "{} : {} assets precached", __FUNCTION__, s_commas( static_cast<float>( engine->asset_cache->cache.size() ) ) ) );
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
