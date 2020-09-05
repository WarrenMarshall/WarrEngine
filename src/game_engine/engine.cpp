
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

bool w_engine::init_game_engine( std::string_view game_name, int argc, char* argv [], w_game* custom_game )
{
	try
	{

#if defined(_FINALRELEASE)
		// in final release, we don't want to bother the user with
		// the visual clutter of the console window
		ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

		{	// LOG FILE

			// get the log file running so we can immediately start writing into it
			logfile = std::make_unique<w_logfile>();
			logfile->init( game_name );
		}

		{ // ENGINE

			engine = std::make_unique<w_engine>();
		}

		logfile->time_stamp( "Started" );

		{	// ENGINE
			log_msg( "Initializing engine" );
			engine->init();
		}

		// #todo : write a proper command line parsing class
		{	// COMMAND LINE

		}

		{	// WINDOW

			log_msg( "Creating window" );
			if( !engine->window->init() )
			{
				return false;
			}
		}

		{	// OPENGL

			log_msg( "Initializing OpenGL" );
			OPENGL->init();
		}

		{	// RENDERER

			log_msg( "Initializing renderer" );
			RENDER->init();
		}

		{	// AUDIO
			log_msg( "Initializing audio" );

			engine->audio_context = cs_make_context(
				glfwGetWin32Window( engine->window->window ),
				44000, 8192, 50, NULL );

			if( !engine->audio_context )
			{
				log_warning( "Audio : init failed!" );
			}
			else
			{
				cs_spawn_mix_thread( engine->audio_context );
				cs_thread_sleep_delay( engine->audio_context, 10 );
			}
		}

		{	// "ASSET DEFINITION / INI" FILES AND PRECACHING

			// read asset definitions and cache them
			log_msg( "Caching asset definitions (*.asset_def)..." );
			engine->cache_asset_definition_files( "game_engine_data" );
			engine->cache_asset_definition_files( fmt::format( "{}_data", game_name ) );

			// this feels like an odd dance, but the idea is that we:
			//
			// 1. parse the asset_def files looking for preprocessor symbols (pass 0)
			// 2. parse the INI files
			// 3. parse the asset_def files again, for the rest of the passes (pass 1+)
			//
			// this ordering is important because step 2 may want to use symbols
			// like "true" or "false" in the INI files.
			//
			// by the time we get to step 3, we have all the symbols from the preproc
			// and the INI files loaded, and the assets can use any symbols they please.

			// do the preprocess pass first so the symbols are in memory
			log_msg( "Precaching resources from definition files..." );
			engine->precache_asset_resources( 0, game_name );

			// parse INI files after the preprocess pass so they can
			// use preprocessor symbols
			log_msg( "Caching configuration (*.ini)..." );
			engine->parse_config_files( "game_engine_data" );
			engine->parse_config_files( fmt::format( "{}_data", game_name ) );

			// put the k/v pairs from the INI files into the global symbol
			// table so they can be referenced by assets in the asset_def files

			for( const auto& [key, value] : engine->config_vars->kv )
			{
				engine->_symbol_to_value[ key ] = value;
			}

			// precache the rest of the assets in the remaining passes
			for( int pass = 1; pass < num_asset_def_passes; ++pass )
			{
				engine->precache_asset_resources( pass, game_name );
			}
		}

		w_random::seed();

		{ // APPLY CONFIG SETTINGS
			v_window_w = w_parser::float_from_str( engine->config_vars->find_value_opt( "v_window_w", "320" ) );
			v_window_h = w_parser::float_from_str( engine->config_vars->find_value_opt( "v_window_h", "240" ) );

			w_rect rc = engine->window->compute_max_window_size_for_desktop();
			glfwSetWindowPos( engine->window->window, static_cast<int>( rc.x ), static_cast<int>( rc.y ) );
			glfwSetWindowSize( engine->window->window, static_cast<int>( rc.w ), static_cast<int>( rc.h ) );
			glfwSetWindowAspectRatio( engine->window->window,
									  100,
									  static_cast<int>( ( v_window_h / v_window_w ) * 100 ) );

			bool vsync = w_parser::bool_from_str( engine->config_vars->find_value_opt( "v_sync", "false" ) );
			log_msg( fmt::format( "VSync: {}", vsync ? "true" : "false" ) );
			glfwSwapInterval( vsync ? 1 : 0 );
			engine->window->set_title( engine->config_vars->find_value_opt( "app_title", "Game Engine" ) );
			glfwSetWindowAttrib( engine->window->window, GLFW_FLOATING, w_parser::bool_from_str( engine->config_vars->find_value_opt( "always_on_top", "false" ) ) );
			engine->window->v_window_clear_color = w_parser::color_from_str( engine->config_vars->find_value_opt( "v_window_clear_color", "64,64,64" ) );
			engine->window->window_clear_color = w_parser::color_from_str( engine->config_vars->find_value_opt( "window_clear_color", "32,32,32" ) );
		}

		// set up frame buffers
		engine->opengl->fb_game = std::make_unique<w_opengl_framebuffer>( "game", v_window_w, v_window_h );

		{ // GAME

			log_msg( "Initializing game" );
			game = custom_game;
			game->init();
			game->new_game();
		}

		{ // INPUT

			log_msg( "Initializing input" );
			engine->input->init();
		}

 		engine->is_running = true;
		engine->time->init();

		// used for wireframe drawing
		engine->white_wire = engine->get_asset<a_subtexture>( "engine_white_wire" );
		engine->white_wire->tex->gl_prim_type = GL_LINES;

		// used for solid drawing
		engine->white_solid = engine->get_asset<a_subtexture>( "engine_white_solid" );

		engine->ui->init();
		engine->ui->theme->init();
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

	log_msg( "Shutting down Audio" );
	if( engine->audio_context )
	{
		cs_stop_all_sounds( engine->audio_context );
		cs_shutdown_context( engine->audio_context );
	}
	engine->audio_context = nullptr;

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

		// whatever remaining ms are left in engine->time->fts_accum_ms should be passed
		// to the render functions for interpolation/prediction
		//
		// it is passed a percentage for easier use : 0.0f-1.0f
		//
		float interp_pct = engine->time->fts_accum_ms / w_time::FTS_step_value_ms;

		// draw the scene to a framebuffer, sized to match the virtual viewport

		engine->opengl->fb_game->bind();
		RENDER->begin_frame( interp_pct );
		{
			glViewport( 0, 0, (int) v_window_w, (int) v_window_h );
			glClearColor( engine->window->window_clear_color.r, engine->window->window_clear_color.g, engine->window->window_clear_color.b, engine->window->window_clear_color.a );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			RENDER->init_projection();

			engine->layer_mgr->draw();
			UI->draw_topmost();
			engine->draw();
		}
		RENDER->end_frame();
		engine->opengl->fb_game->unbind();

		// reset the viewport to the size of the actual window and draw the
		// offscreen framebuffer to the actual framebuffer as a scaled quad

		glViewport(
			static_cast<int>( engine->window->viewport_pos_sz.x ), static_cast<int>( engine->window->viewport_pos_sz.y ),
			static_cast<int>( engine->window->viewport_pos_sz.w ), static_cast<int>( engine->window->viewport_pos_sz.h )
		);

		glClearColor( engine->window->window_clear_color.r, engine->window->window_clear_color.g, engine->window->window_clear_color.b, engine->window->window_clear_color.a );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		static a_texture* tex = engine->get_asset<a_texture>( "tex_game_frame_buffer" );

		RENDER
			->begin()
			->draw( tex, w_rect( 0, 0 ) )
			->end();
		RENDER->maybe_draw_master_buffer( nullptr );

		// we're done, swap the buffers!

		glfwSwapBuffers( engine->window->window );
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
	returns a string containing the value stored for 'symbol'
*/
std::string w_engine::find_string_from_symbol( std::string_view symbol )
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
	std::string sval = find_string_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<int>( strtol( sval.data(), ( char** ) nullptr, 10 ) );
}

float w_engine::find_float_from_symbol( std::string_view symbol, float def_value )
{
	std::string sval = find_string_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<float>( strtof( sval.data(), ( char** ) nullptr ) );
}

w_color w_engine::find_color_from_symbol( std::string_view symbol, w_color def_value )
{
	std::string sval = find_string_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a );
	}

	return w_color( sval );
}

w_range w_engine::find_range_from_symbol( std::string_view symbol, w_range def_value )
{
	std::string sval = find_string_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{},{}", def_value.min, def_value.max );
	}

	return w_range( sval );
}

w_vec2 w_engine::find_vec2_from_symbol( std::string_view symbol, w_vec2 def_value )
{
	std::string sval = find_string_from_symbol( symbol );

	if( sval == str_not_found )
	{
		sval = fmt::format( "{},{}", def_value.x, def_value.y );
	}

	return w_vec2( sval );
}

w_vec3 w_engine::find_vec3_from_symbol( std::string_view symbol, w_vec3 def_value )
{
	std::string sval = find_string_from_symbol( symbol );

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
	config_vars = std::make_unique<w_keyvalues>();

	input->add_listener( this );
	input->add_listener( layer_mgr.get() );

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
		RENDER
			->begin()
			->push_depth( zdepth_engine );

		w_vec2 v0, v1, v2, v3;

		v0 = w_vec2::zero;
		v1 = w_vec2( v_window_w - 1, 0.0f );
		v2 = w_vec2( v_window_w - 1, v_window_h - 1 );
		v3 = w_vec2( 0.0f, v_window_h - 1 );

		RENDER->push_rgb( w_color::black );
		RENDER->draw_line( v0, v1 );
		RENDER->draw_line( v1, v2 );
		RENDER->draw_line( v2, v3 );
		RENDER->draw_line( v3, v0 );

		v0 = w_vec2( 1, 1 );
		v1 = w_vec2( v_window_w - 2, 1.0f );
		v2 = w_vec2( v_window_w - 2, v_window_h - 2 );
		v3 = w_vec2( 1.0f, v_window_h - 2 );

		RENDER->push_rgb( w_color::orange );
		RENDER->draw_line( v0, v1 );
		RENDER->draw_line( v1, v2 );
		RENDER->draw_line( v2, v3 );
		RENDER->draw_line( v3, v0 );

		v0 = w_vec2( 2.0f, 2.0f );
		v1 = w_vec2( v_window_w - 3, 2.0f );
		v2 = w_vec2( v_window_w - 3, v_window_h - 3 );
		v3 = w_vec2( 2.0f, v_window_h - 3 );

		RENDER->push_rgb( w_color::black );
		RENDER->draw_line( v0, v1 );
		RENDER->draw_line( v1, v2 );
		RENDER->draw_line( v2, v3 );
		RENDER->draw_line( v3, v0 );

		RENDER->end();
	}
}

void w_engine::update()
{
	layer_mgr->update();

	for( const auto& [xxx, asset] : asset_cache->cache )
	{
		asset->update();
	}

	render->show_stats = input->is_button_down( input_id::key_s);
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

void w_engine::parse_config_files( const std::string_view folder_name )
{
	std::vector<std::string> filenames;
	engine->fs->scan_folder_for_ext( filenames, fmt::format( "{}", folder_name ), ".ini" );

	for( const auto& iter : filenames )
	{
		engine->parse_config_file( iter );
	}
}

void w_engine::parse_config_file( std::string_view filename )
{
	auto file = engine->fs->load_text_file_into_memory( filename );

	for( const auto& line : *( file.get()->lines.get() ) )
	{
		if( line.starts_with( "\"" ) )
		{
			w_tokenizer tok_kv( line, '\"' );

			tok_kv.get_next_token();
			std::string_view key = tok_kv.get_next_token();
			tok_kv.get_next_token();
			std::string_view value = tok_kv.get_next_token();

			if( key.length() && value.length() )
			{
				config_vars->set( key, value );
			}
		}
	}
}

/*
	iterate through all the cached asset_definition_files and look
	for any asset_definitions that have assets we need to precache

	things like texture files, sound files, etc.
*/
void w_engine::precache_asset_resources( int pass, std::string_view game_name )
{
	for( const auto& asset_definition_file : engine->asset_definition_file_cache->cache )
	{
		asset_definition_file->precache_asset_resources( pass );
	}

	log_msg( fmt::format( "{} : pass: {} / {} assets precached", __FUNCTION__, pass, s_commas( static_cast<float>( engine->asset_cache->cache.size() ) ) ) );
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
			}
		}
		break;
	}
}
