
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

bool w_engine::init_game_engine( int argc, char* argv [] )
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
			logfile->init( base_game->name );
		}

		{ // ENGINE

			engine = std::make_unique<w_engine>();
		}

		logfile->time_stamp( "Started" );

		{	// ENGINE
			log( "Initializing engine" );
			engine->init();

			// if the paths we expect to be on the disk are not there, create them. this mitigates
			// problems later on if the app wants to, say, save a file and would have problems
			// if the data folder wasn't there.

			w_file_system::create_path_if_not_exist( "data/warr_engine" );
			w_file_system::create_path_if_not_exist( fmt::format( "data/{}", base_game->get_game_name() ) );
		}

		{	// COMMAND LINE

		}

		{	// WINDOW

			log( "Creating window" );
			if( !engine->window->init() )
			{
				return false;
			}
		}

		{	// OPENGL

			log( "Initializing OpenGL" );
			OPENGL->init();
		}

		{	// RENDERER

			log( "Initializing renderer" );
			RENDER->init();
		}

		{	// AUDIO
		#ifdef USE_BASS_SOUND_LIBRARY
			log( "Initializing BASS audio" );
			if( !BASS_Init( -1, 44100, 0, nullptr, nullptr ) )
			{
				log_warning( "BASS : Audio init failed!" );
			}
		#else
			log( "Initializing Cute_Sound audio" );
			engine->c2_sound_context = cs_make_context(
				glfwGetWin32Window( engine->window->window ),
				44100, 8192, 150, NULL );

			if( !engine->c2_sound_context )
			{
				log_warning( "Cute_Sound : Audio init failed!" );
			}
			else
			{
				cs_spawn_mix_thread( engine->c2_sound_context );
			}
		#endif
		}

		{	// "ASSET DEFINITION & INI" FILES AND PRECACHING

			// read asset definitions and cache them
			log( "Caching asset definitions (*.asset_def)..." );
			engine->cache_asset_definition_files( "data/warr_engine" );
			engine->cache_asset_definition_files( fmt::format( "data/{}", base_game->name ) );

			// this feels like an odd dance, but the idea is that we:
			//
			// 1. parse the asset_def files looking for preprocessor symbols (pass 0)
			// 2. parse the INI files
			// 3. parse the asset_def files again, for the rest of the passes (pass 1+)
			//
			// this ordering is important because step 2 may want to use symbols
			// like "true" or "false" or "v_window_h" in the INI files.
			//
			// by the time we get to step 3, we have all the symbols from the preproc
			// and the INI files loaded, and the asset_def files can use any symbols they please.

			// do the preprocess pass first so the symbols are in memory
			log( "Precaching resources from definition files..." );
			engine->precache_asset_resources( 0, base_game->name );
			engine->precache_asset_resources( 1, base_game->name );

			// parse INI files after the preprocess pass so they can
			// use preprocessor symbols
			log( "Caching configuration (*.ini)..." );
			engine->parse_config_files( "data/warr_engine" );
			engine->parse_config_files( fmt::format( "data/{}", base_game->name ) );

			// put the k/v pairs from the INI files into the global symbol
			// table so they can be referenced by assets in the asset_def files

			for( const auto& [key, value] : engine->config_vars->kv )
			{
				engine->_symbol_to_value[ key ] = value;
			}
		}

		{ // APPLY CONFIG SETTINGS

			w_tokenizer tok;

			tok.init( engine->config_vars->find_value_opt( "v_window_res", "320x240" ), 'x' );
			v_window_w = w_parser::float_from_str( tok.tokens[ 0 ] );
			v_window_h = w_parser::float_from_str( tok.tokens[ 1 ] );
			log( "V Window Res: {}x{}", (int) v_window_w, (int) v_window_h );

			tok.init( engine->config_vars->find_value_opt( "ui_canvas_res", "640x480" ), 'x' );
			ui_canvas_w = w_parser::float_from_str( tok.tokens[ 0 ] );
			ui_canvas_h = w_parser::float_from_str( tok.tokens[ 1 ] );
			log( "UI Canvas Res: {}x{}", (int) ui_canvas_w, (int) ui_canvas_h );

			RENDER->palette = a_palette::find( engine->config_vars->find_value_opt( "palette_tag", "pal_default" ) );

			w_rect rc = engine->window->compute_max_window_size_for_desktop();
			glfwSetWindowPos( engine->window->window, static_cast<int>( rc.x ), static_cast<int>( rc.y ) );
			glfwSetWindowSize( engine->window->window, static_cast<int>( rc.w ), static_cast<int>( rc.h ) );
			glfwSetWindowAspectRatio( engine->window->window,
									  100,
									  static_cast<int>( ( v_window_h / v_window_w ) * 100 ) );

			bool vsync = w_parser::bool_from_str( engine->config_vars->find_value_opt( "v_sync", "false" ) );
			log( "VSync: {}", vsync ? "true" : "false" );
			glfwSwapInterval( vsync ? 1 : 0 );
			engine->window->set_title( engine->config_vars->find_value_opt( "app_title", "Game Engine" ) );
			glfwSetWindowAttrib( engine->window->window, GLFW_FLOATING, w_parser::bool_from_str( engine->config_vars->find_value_opt( "always_on_top", "false" ) ) );
			engine->window->v_window_clear_color = w_parser::color_from_str( engine->config_vars->find_value_opt( "v_window_clear_color", "64,64,64" ) );
			engine->window->window_clear_color = w_parser::color_from_str( engine->config_vars->find_value_opt( "window_clear_color", "32,32,32" ) );
		}

		{ // FINISH ASSET PRECACHE

			engine->precache_asset_resources( 2, base_game->name );
			engine->precache_asset_resources( 3, base_game->name );
		}

		{ // Box2D

			log( "Initializing Box2D" );
			engine->new_physics_world();
		}

		// set up frame buffers
		engine->opengl->fb_game = std::make_unique<w_opengl_framebuffer>( "game", v_window_w, v_window_h );

		{ // GAME

			log( "Initializing game" );
			base_game->init();
			base_game->reset_layer_stack_to_main_menu();
		}

		{ // INPUT

			log( "Initializing input" );
			engine->input->init();
		}

 		engine->is_running = true;
		engine->time->init();

		// used for wireframe drawing
		engine->white_wire = a_subtexture::find( "engine_white_wire" );
		engine->white_wire->tex->gl_prim_type = GL_LINES;

		// used for solid drawing
		engine->white_solid = a_subtexture::find( "engine_white_solid" );

		// the texture we are rendering to each frame
		engine->tex_frame_buffer = a_texture::find( "tex_game_frame_buffer" );

		// there's a simple pixel font that always lives inside of engine so
		// there is always a font available, regardless of ui theme settings.
		engine->pixel_font = a_font::find( "font_ui" );

		engine->ui->init();
	}
	catch( std::exception& e )
	{
		log( "!! EXCEPTION CAUGHT !!" );
		log( "\t{}", e.what() );

		MessageBoxA( nullptr, e.what(), "Exception!", MB_OK );
	}

	return true;
}

void w_engine::deinit_game_engine()
{
	// Clean up

	log( "Shutting down..." );

	// this needs to be done before the audio or windowing systems, to give
	// the layers a chance to clean up first.
	log( "Shutting down layer manager" );
	engine->layer_mgr->clear_stack();

	log( "Shutting down window" );
	engine->window->deinit();

	log( "Shutting down OpenGL" );
	for( auto& shader : engine->opengl->shader_pool )
	{
		glDeleteProgram( shader.second->id );
	}

	log( "Shutting down GLFW" );
	glfwTerminate();

	log( "Shutting down Audio" );
#ifdef USE_BASS_SOUND_LIBRARY
	BASS_Free();
#else
	if( engine->c2_sound_context )
	{
		cs_stop_all_sounds( engine->c2_sound_context );
		cs_shutdown_context( engine->c2_sound_context );
		engine->c2_sound_context = nullptr;
	}
#endif

	log( "Shutting down input" );
	engine->input->deinit();

	log( "Shutting down engine" );
	engine->deinit();

	// Do this last so we can log right up until the last moment
	logfile->time_stamp( "Ended" );
	log( "Finished!" );
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

		// whatever remaining ms are left in engine->time->fts_accum_ms should be passed
		// to the render functions for interpolation/prediction
		//
		// it is passed a percentage for easier use : 0.0f-1.0f

		RENDER->frame_interpolate_pct = engine->time->fts_accum_ms / w_time::FTS_step_value_ms;

		/*
			process user input
		*/
		engine->input->queue_presses();

		IMGUI->reset();

		/*
			if we have fixed time steps to perform, walk
			through them one at a time
		*/

		while( engine->time->fts_accum_ms >= w_time::FTS_step_value_ms )
		{
			engine->time->fts_accum_ms -= w_time::FTS_step_value_ms;

			engine->input->queue_motion();
			engine->input->update();

			engine->box2d_world->Step( w_time::FTS_step_value_s, b2d_velocity_iterations, b2d_position_iterations );

			engine->update();
			engine->render->stats.update();
			base_game->update();
		}

		// #shader_refactor - wtf is this?
		static float time_val = 0.0f;
		time_val += engine->time->delta_ms / 2000.f;
		OPENGL->set_uniform( "in_time", time_val );
		OPENGL->set_uniform( "in_use_vignette", 1.0f );

		// draw the scene to a framebuffer, sized to match the virtual viewport

		engine->opengl->fb_game->bind();
		RENDER->begin_frame();
		{
			glViewport( 0, 0, (int) v_window_w, (int) v_window_h );
			glClearColor( engine->window->window_clear_color.r, engine->window->window_clear_color.g, engine->window->window_clear_color.b, engine->window->window_clear_color.a );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			OPENGL->init_projection_matrix();
			OPENGL->init_view_matrix_identity();
			OPENGL->find_shader( "simple" )->bind();

			// ----------------------------------------------------------------------------
			// render the frame

			// layers and entities
			engine->layer_mgr->draw();

			OPENGL->init_view_matrix_identity();

			// top most UI elements, like the mouse cursor
			UI->draw_topmost();

			// engine specific things, like pause borders
			engine->draw();
		}

		OPENGL->init_view_matrix_identity_ui();
		RENDER->end_frame();

		engine->opengl->fb_game->unbind();

		// reset the viewport to the size of the actual window and draw the
		// offscreen framebuffer to the actual framebuffer as a scaled quad

		OPENGL->init_view_matrix_identity();

		glViewport(
			static_cast<int>( engine->window->viewport_pos_sz.x ), static_cast<int>( engine->window->viewport_pos_sz.y ),
			static_cast<int>( engine->window->viewport_pos_sz.w ), static_cast<int>( engine->window->viewport_pos_sz.h )
		);

		glClearColor( engine->window->window_clear_color.r, engine->window->window_clear_color.g, engine->window->window_clear_color.b, engine->window->window_clear_color.a );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#if 0
		OPENGL->find_shader( "crt_fx" )->bind();
#endif

		RENDER
			->begin()
			->draw( engine->tex_frame_buffer, w_rect( 0, 0 ) )
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
std::optional<std::string> w_engine::find_string_from_symbol( std::string_view symbol )
{
	if( !is_symbol_in_map( symbol ) )
	{
		return std::nullopt;
	}

	return _symbol_to_value[ std::string( symbol ) ];
}

/*
	returns a value to the caller based on the contents of 'symbol'

	if no value is found for 'symbol', the default value is returned
*/
int w_engine::find_int_from_symbol( std::string_view symbol, int def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{}", def_value );
	}

	return str_to_int( std::string( *sval ) );
}

float w_engine::find_float_from_symbol( std::string_view symbol, float def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{}", def_value );
	}

	return static_cast<float>( strtof( std::string(*sval).data(), ( char** ) nullptr ) );
}

w_color w_engine::find_color_from_symbol( std::string_view symbol, w_color def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a );
	}

	return w_color( *sval );
}

w_range w_engine::find_range_from_symbol( std::string_view symbol, w_range def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{},{}", def_value.start, def_value.end );
	}

	return w_range( *sval );
}

w_vec2 w_engine::find_vec2_from_symbol( std::string_view symbol, w_vec2 def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{},{}", def_value.x, def_value.y );
	}

	return w_vec2( *sval );
}

void w_engine::new_physics_world()
{
	engine->box2d_world = std::make_unique<b2World>( b2Vec2( 0.0f, b2d_gravity_default ) );

	engine->physics_debug_draw = std::make_unique<w_physics_debug_draw>();
	engine->box2d_world->SetDebugDraw( engine->physics_debug_draw.get() );
	engine->physics_debug_draw->SetFlags( b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit );

#ifdef _DEBUG
	RENDER->show_physics_debug = true;
#else
	RENDER->show_physics_debug = false;
#endif
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
	opengl = std::make_unique<w_opengl>();
	config_vars = std::make_unique<w_keyvalues>();
	random = std::make_unique<w_random>();

	fs->init();
}

void w_engine::deinit()
{
}

void w_engine::draw()
{
	// If engine is paused, draw a border around the screen for visibility

	if( is_paused )
	{
		RENDER
			->begin()
			->push_depth( zdepth_topmost );

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
	engine->fs->scan_folder_for_ext( &filenames, fmt::format( "{}", folder_name ), ".asset_def" );

	for( const auto& iter : filenames )
	{
		engine->asset_definition_file_cache->add( iter );
	}
}

void w_engine::parse_config_files( const std::string_view folder_name )
{
	std::vector<std::string> filenames;
	engine->fs->scan_folder_for_ext( &filenames, fmt::format( "{}", folder_name ), ".ini" );

	for( const auto& iter : filenames )
	{
		engine->parse_config_file( iter );
	}
}

void w_engine::parse_config_file( std::string_view filename )
{
	auto file = engine->fs->load_text_file_into_memory( filename );

	for( const auto& line : *( file->lines.get() ) )
	{
		if( line.substr( 0, 1 ) == "\"" )
		{
			w_tokenizer tok_kv( line, '\"' );

			auto key = tok_kv.get_next_token();
			tok_kv.get_next_token(); // skip blank
			auto value = tok_kv.get_next_token();

			if( key.has_value() && value.has_value() )
			{
				config_vars->insert_or_assign( *key, *value );
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

	log( "Pass: {} / {} assets precached", pass, f_commas( static_cast<float>( engine->asset_cache->cache.size() ) ) );
}

bool w_engine::iir_on_released( const w_input_event* evt )
{
	// toggle engine pause
	if( evt->input_id == input_id::key_pause )
	{
		toggle_pause();
		return true;
	}

	// slow down game clock
	if( evt->input_id == input_id::key_left_bracket )
	{
		time->dilation -= 0.1f;
		time->dilation = glm::max( time->dilation, 0.1f );

		if( engine->input->is_shift_down() )
		{
			time->dilation = 1.0f;
		}
		return true;
	}

	// speed up game clock
	if( evt->input_id == input_id::key_right_bracket )
	{
		time->dilation += 0.1f;

		if( engine->input->is_shift_down() )
		{
			time->dilation = 5.0f;
		}
		return true;
	}

	// toggle full screen
	if( evt->input_id == input_id::key_f11 )
	{
		window->toggle_fullscreen();
		return true;
	}

 	if( evt->input_id == input_id::key_enter )
	{
		if( engine->input->is_alt_down() )
		{
			window->toggle_fullscreen();
		}
		return true;
	}

	// toggle esc menu
	if( evt->input_id == input_id::key_esc )
	{
		if( typeid( *layer_mgr->get_top() ) == typeid( layer_esc_menu ) )
		{
			layer_mgr->pop();
		}
		else
		{
			layer_mgr->push<layer_esc_menu>();
		}
		return true;
	}

	// toggle debug physics drawing
	if( evt->input_id == input_id::key_f5 )
	{
		RENDER->show_physics_debug = !RENDER->show_physics_debug;
		return true;
	}

	return false;
}
