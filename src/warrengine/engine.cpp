
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_engine::launch( int argc, char* argv [] )
{
#if defined(_FINALRELEASE)
	// in final release, we don't want to bother the user with the visual
	// clutter of the console window
	ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

	{	// LOG FILE

		// get the log file running so we can immediately start writing into it

		logfile = std::make_unique<w_logfile>();
		logfile->init( base_game->name );
		log( "Logging started" );
	}

	{ // ENGINE

		log( "Creating engine instance" );
		engine = std::make_unique<w_engine>();
	}

	{	// ENGINE
		log( "Initializing engine" );
		engine->init();

		// if the paths we expect to be on the disk are not there, create them.
		// this mitigates problems later on if the app wants to, say, save a
		// file and would have problems if the data folder wasn't there.

		w_file_system::create_path_if_not_exist( "data/warr_engine" );
		w_file_system::create_path_if_not_exist( fmt::format( "data/{}", base_game->get_game_name() ) );
	}

	// COMMAND LINE
	{
		for( auto x = 1 ; x < argc ; ++x )
		{
			std::string arg = argv[ x ];

			if( arg == "-verbose" )
			{
				engine->cmdline.verbose = true;
				log_verbose( "cmdline : \"{}\" : Verbose logging enabled.", arg );
			}

			if( arg == "-nobatch" )
			{
				engine->cmdline.nobatch = true;
				log_verbose( "cmdline : \"{}\" : Batch rendering disabled.", arg );
			}
		}
	}

	{	// WINDOW

		log( "Creating main window" );
		engine->window->init();
	}

	{	// OPENGL

		log( "Initializing OpenGL" );
		engine->opengl->init();
	}

	{	// RENDERER

		log( "Initializing renderer" );
		engine->render->init();
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
		// like "true" or "false" or "viewport_h" in the INI files.
		//
		// by the time we get to step 3, we have all the symbols from the
		// preproc and the INI files loaded, and the asset_def files can use any
		// symbols they please.

		// do the preprocess pass first so the symbols are in memory

		log( "Precaching resources from definition files..." );
		engine->precache_asset_resources( 0 );
		engine->precache_asset_resources( 1 );

		// parse INI files after the preprocess pass so they can use
		// preprocessor symbols

		log( "Caching configuration (*.ini)..." );
		engine->parse_config_files( "data/warr_engine" );
		engine->parse_config_files( fmt::format( "data/{}", base_game->name ) );

		// put the k/v pairs from the INI files into the global symbol table so
		// they can be referenced by assets in the asset_def files

		for( const auto& [key, value] : engine->config_vars->kv )
		{
			engine->_symbol_to_value[ key ] = value;
		}
	}

	{ // APPLY CONFIG SETTINGS

		w_tokenizer tok;

		tok.init( engine->config_vars->find_value_opt( "viewport_res", "320x240" ), 'x' );
		viewport_w = w_parser::float_from_str( tok.tokens[ 0 ] );
		viewport_h = w_parser::float_from_str( tok.tokens[ 1 ] );
		engine->_symbol_to_value[ "viewport_w" ] = fmt::format( "{}", viewport_w );
		engine->_symbol_to_value[ "viewport_h" ] = fmt::format( "{}", viewport_h );
		engine->_symbol_to_value[ "viewport_hw" ] = fmt::format( "{}", viewport_hw );
		engine->_symbol_to_value[ "viewport_hh" ] = fmt::format( "{}", viewport_hh );
		log( "V Window Res: {}x{}", (int) viewport_w, (int) viewport_h );

		tok.init( engine->config_vars->find_value_opt( "ui_res", "640x480" ), 'x' );
		ui_w = w_parser::float_from_str( tok.tokens[ 0 ] );
		ui_h = w_parser::float_from_str( tok.tokens[ 1 ] );
		engine->_symbol_to_value[ "ui_w" ] = fmt::format( "{}", ui_w );
		engine->_symbol_to_value[ "ui_h" ] = fmt::format( "{}", ui_h );
		engine->_symbol_to_value[ "ui_hw" ] = fmt::format( "{}", ui_hw );
		engine->_symbol_to_value[ "ui_hh" ] = fmt::format( "{}", ui_hh );
		log( "UI Window Res: {}x{}", (int) ui_w, (int) ui_h );

		engine->render->palette = a_palette::find( engine->config_vars->find_value_opt( "palette_tag", "pal_default" ) );

		w_rect rc = engine->window->compute_max_window_size_for_desktop();
		glfwSetWindowPos( engine->window->glfw_window, static_cast<int>( rc.x ), static_cast<int>( rc.y ) );
		glfwSetWindowSize( engine->window->glfw_window, static_cast<int>( rc.w ), static_cast<int>( rc.h ) );
		glfwSetWindowAspectRatio( engine->window->glfw_window,
			100,
			static_cast<int>( ( viewport_h / viewport_w ) * 100 ) );

		bool vsync = w_parser::bool_from_str( engine->config_vars->find_value_opt( "v_sync", "false" ) );
		log( "VSync: {}", vsync ? "true" : "false" );
		glfwSwapInterval( vsync ? 1 : 0 );
		engine->window->set_title( engine->config_vars->find_value_opt( "app_title", "Game Engine" ) );
		glfwSetWindowAttrib( engine->window->glfw_window, GLFW_FLOATING, w_parser::bool_from_str( engine->config_vars->find_value_opt( "always_on_top", "false" ) ) );
		engine->window->viewport_clear_color = w_parser::color_from_str( engine->config_vars->find_value_opt( "viewport_clear_color", "64,64,64" ) );
		engine->window->window_clear_color = w_parser::color_from_str( engine->config_vars->find_value_opt( "window_clear_color", "32,32,32" ) );
	}

	{	// finish precaching

		engine->precache_asset_resources( 2 );
		engine->precache_asset_resources( 3 );
	}

	{
		// finalize the asset precache by allowing assets to do things that
		// aren't thread safe
		for( auto& asset : engine->asset_cache->cache )
		{
			asset.second->finalize_after_loading();
		}
	}

	{ // Box2D

		log( "Initializing physics" );
		engine->new_physics_world();
	}

	// set up frame buffers

	engine->frame_buffer = std::make_unique<w_opengl_framebuffer>( "game" );
	engine->frame_buffer->add_color_attachment( engine->window->window_clear_color );
	engine->frame_buffer->add_color_attachment();
	engine->frame_buffer->add_color_attachment();
	engine->frame_buffer->finalize();

	engine->blur_frame_buffers[ 0 ] = std::make_unique<w_opengl_framebuffer>( "blur1" );
	engine->blur_frame_buffers[ 0 ]->add_color_attachment();
	engine->blur_frame_buffers[ 0 ]->finalize();

	engine->blur_frame_buffers[ 1 ] = std::make_unique<w_opengl_framebuffer>( "blur2" );
	engine->blur_frame_buffers[ 1 ]->add_color_attachment();
	engine->blur_frame_buffers[ 1 ]->finalize();

	engine->composite_frame_buffer = std::make_unique<w_opengl_framebuffer>( "composite" );
	engine->composite_frame_buffer->add_color_attachment();
	engine->composite_frame_buffer->finalize();

	// used for solid drawing
	engine->tex_white = a_texture::find( "engine_white" );

	// there's a simple pixel font that always lives inside of engine so there
	// is always a font available, regardless of ui theme settings.
	engine->pixel_font = a_font::find( "engine_pixel_font" );

	{ // GAME

		log( "Initializing game" );
		base_game->init();
		//base_game->reset_layer_stack_to_main_menu();
	}

	{ // INPUT

		log( "Initializing input" );
		engine->input->init();
	}

	engine->is_running = true;
	engine->time->init();

	engine->ui->init();

	log( "Running..." );

	engine->main_loop();

	log( "Shutting down..." );
	engine->shutdown();
}

void w_engine::shutdown()
{
	// Clean up
	log( "Shutting down..." );

	// this needs to be done before the audio or windowing systems, to give the
	// layers a chance to clean up first.

	log( "Shutting down layer manager" );
	layer_mgr->clear_stack();

	log( "Shutting down window" );
	window->deinit();

	log( "Shutting down GLFW" );
	glfwTerminate();

	log( "Shutting down OpenGL" );
	for( auto& iter : engine->opengl->shaders )
	{
		auto& shader = iter.second;
		glDeleteProgram( shader.id );
	}

	log( "Shutting down input" );
	input->deinit();

	log( "Shutting down engine" );
	deinit();

	// Do this last so we can log right up until the last moment
	logfile->time_stamp( "Ended" );
	logfile->deinit();
}

void w_engine::main_loop()
{
	while( is_running && !glfwWindowShouldClose( window->glfw_window ) )
	{
		// update core engine stuff - time, timers, etc

		time->update();
		engine->ui->imgui->reset();

		// whatever remaining ms are left in time->fts_accum_ms should be passed
		// to the render functions for interpolation/prediction
		//
		// it is passed a percentage for easier use : 0.0f-1.0f

		engine->render->frame_interpolate_pct = time->fts_accum_ms / (float)fixed_time_step::ms_per_step;

		// queue up inputs for processing later in the loop
		input->queue_presses();

		// if the engine is paused, we need to continue processing user input so
		// that the ESC menu and engine can respond to keypresses

		if( is_paused() )
		{
			input->update();
		}

		// if we have fixed time steps to perform, walk through them one at a time

		while( time->fts_accum_ms >= fixed_time_step::ms_per_step )
		{
			time->fts_accum_ms -= fixed_time_step::ms_per_step;

			// queue up inputs for processing later in the loop
			input->queue_motion();

			box2d_world->Step( fixed_time_step::per_second_scaler, b2d_velocity_iterations, b2d_pos_iterations );

			process_collision_queue();

			update();
			engine->stats->update();
			base_game->update();
		}

		// update shader parameters
		engine->opengl->set_uniform( "u_current_time", (float)time->now() / 1000.f );

		// ----------------------------------------------------------------------------
		// draw the scene to the engine frame buffer
		//
		// this draws several versions of the scene at once:
		//
		// 1. the scene as normal
		// 2. same as #1 but only contains the brightest pixels (used for glow effects)

		glEnable( GL_DEPTH_TEST );
		frame_buffer->bind();

		engine->opengl->shaders[ "base_with_glow" ].bind();

		engine->render->begin_frame();
		{
			// ----------------------------------------------------------------------------
			// render the game frame
			// ----------------------------------------------------------------------------

			engine->opengl->init_projection_matrix();
			engine->opengl->init_view_matrix_identity();

			{
				scoped_opengl;

				// layers and entities
				layer_mgr->draw();
			}

			// engine specific things, like pause borders
			draw();
			engine->render->draw_and_reset_all_batches();
		}
		engine->render->end_frame();
		frame_buffer->unbind();

		// process the input that queued earlier AFTER the rendering has taken place
		// so that the imgui code has a chance to respond. the ui has to be drawn
		// before it can react to input.

		input->update();

		// ----------------------------------------------------------------------------
		// the engine frame buffer now contains the color texture and the
		// brightness texture
		//
		// draw the brightness texture into the blur frame buffer, using the
		// blur shader
		// ----------------------------------------------------------------------------

		glDisable( GL_DEPTH_TEST );

		engine->opengl->init_view_matrix_identity();
		blur_frame_buffers[ 0 ]->bind();
		engine->opengl->shaders[ "blur" ].bind();
		engine->opengl->set_uniform( "horizontal", false );
		w_render::draw_quad( frame_buffer->color_attachments[ 1 ].texture, w_rect( 0.0f, 0.0f, viewport_w, viewport_h ) );
		engine->render->batch_quads->draw_and_reset_internal();
		blur_frame_buffers[ 0 ]->unbind();

		// pingpong back and forth between the 2 blur frame buffers, blurring
		// them into each other, for a set amount of passes.

		constexpr auto blur_passes = 6;

		bool pingpong = true;
		for( int x = 0 ; x < blur_passes ; ++x )
		{
			blur_frame_buffers[ pingpong ]->bind();
			engine->opengl->set_uniform( "horizontal", pingpong );
			w_render::draw_quad( blur_frame_buffers[ !pingpong ]->color_attachments[ 0 ].texture, w_rect( 0.0f, 0.0f, viewport_w, viewport_h ) );
			engine->render->batch_quads->draw_and_reset_internal();
			blur_frame_buffers[ pingpong ]->unbind();

			pingpong = !pingpong;
		}

		// ----------------------------------------------------------------------------
		// draw the base/bloom frame buffers into the compositing frame buffer
		// ----------------------------------------------------------------------------

		{
			scoped_render_state;

			composite_frame_buffer->bind();

			// draw game frame buffer

			engine->opengl->shaders[ "base" ].bind();

			w_render::draw_quad( frame_buffer->color_attachments[ 0 ].texture, w_rect( 0.0f, 0.0f, viewport_w, viewport_h ) );
			engine->render->batch_quads->draw_and_reset_internal();

			// draw glow frame buffer on top with blending

			engine->opengl->set_blend( opengl_blend::add );

			w_render::draw_quad( blur_frame_buffers[ 0 ]->color_attachments[ 0 ].texture, w_rect( 0.0f, 0.0f, viewport_w, viewport_h ) );
			engine->render->batch_quads->draw_and_reset_internal();

			engine->opengl->set_blend( opengl_blend::alpha );

			composite_frame_buffer->unbind();
		}

		// ----------------------------------------------------------------------------
		// draw the compositing frame buffer to the default frame buffer
		// applying any screen based post process effects.
		// ----------------------------------------------------------------------------

		engine->opengl->shaders[ "post_process" ].bind();
		engine->opengl->init_view_matrix_identity();

		// reset the viewport to the size of the actual window size
		glViewport(
			(int)window->viewport_pos_sz.x,
			(int)window->viewport_pos_sz.y,
			(int)window->viewport_pos_sz.w,
			(int)window->viewport_pos_sz.h
		);

		w_render::draw_quad( composite_frame_buffer->color_attachments[ 0 ].texture, w_rect( 0.0f, 0.0f, viewport_w, viewport_h ) );
		engine->render->batch_quads->draw_and_reset_internal();

	#if 0
		// ---------------------------------------------------------------------------
		// debug helper
		//
		// draw all the color buffers in little quads at the bottom of the
		// window
		// ----------------------------------------------------------------------------

		engine->opengl->shaders[ "base" ].bind();

		float w = viewport_w / 4.0f;
		float h = viewport_h / 4.0f;
		w_rect rc = { 0.0f, viewport_h - h, w, h };

	#if 0
		// main
		{
			scoped_render_push_pop;

			w_render::draw( frame_buffer->color_attachments[ 0 ].texture, rc );
			render_state.scale = 0.5f;
			w_render::draw_string( "(base)", { rc.x, rc.y } );
			engine->render->batch_quads->vertex_array_object->draw_and_reset_internal();
			rc.x += w;
		}
	#endif

	#if 1
		// glow
		{
			scoped_render_push_pop;

			w_render::draw( frame_buffer->color_attachments[ 1 ].texture, rc );
			render_state.scale = 0.5f;
			w_render::draw_string( "(glow)", { rc.x, rc.y } );
			engine->render->batch_quads->vertex_array_object->draw_and_reset_internal();
			rc.x += w;
		}
	#endif

	#if 1
		// pick
		{
			scoped_render_push_pop;

			w_render::draw( frame_buffer->color_attachments[ 2 ].texture, rc );
			render_state.scale = 0.5f;
			w_render::draw_string( "(pick)", { rc.x, rc.y } );
			engine->render->batch_quads->vertex_array_object->draw_and_reset_internal();
			rc.x += w;
		}
	#endif

	#if 0
		// blurred glow
		{
			scoped_render_push_pop;

			w_render::draw( blur_frame_buffers[ 0 ]->color_attachments[ 0 ].texture, rc );
			render_state.scale = 0.5f;
			w_render::draw_string( "(blur)", { rc.x, rc.y } );
			engine->render->batch_quads->vertex_array_object->draw_and_reset_internal();
			rc.x += w;
		}
	#endif
	#endif

		// ----------------------------------------------------------------------------
		// everything has been drawn the default frame buffer, so let's swap
		// ----------------------------------------------------------------------------

		glfwSwapBuffers( window->glfw_window );
		glfwPollEvents();
	}
}

// checks if 'symbol' exists in the map

bool w_engine::is_symbol_in_map( const std::string_view symbol )
{
	return _symbol_to_value.count( std::string( symbol ) ) > 0;
}

// returns a string containing the value stored for 'symbol'
std::optional<std::string> w_engine::find_string_from_symbol( std::string_view symbol )
{
	if( !is_symbol_in_map( symbol ) )
	{
		return std::nullopt;
	}

	return _symbol_to_value[ std::string( symbol ) ];
}

// returns a value to the caller based on the contents of 'symbol'
//
// if no value is found for 'symbol', the default value is returned

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

	return static_cast<float>( strtof( std::string( *sval ).data(), ( char** ) nullptr ) );
}

w_color w_engine::find_color_from_symbol( std::string_view symbol, const w_color& def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a );
	}

	return w_color( *sval );
}

w_range w_engine::find_range_from_symbol( std::string_view symbol, const w_range& def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = fmt::format( "{},{}", def_value.start, def_value.end );
	}

	return w_range( *sval );
}

w_vec2 w_engine::find_vec2_from_symbol( std::string_view symbol, const w_vec2& def_value )
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
	engine->physics_responder = std::make_unique<w_physics_contact_listener>();
	engine->box2d_world->SetContactListener( engine->physics_responder.get() );

	engine->physics_debug_draw = std::make_unique<w_physics_debug_draw>();
	engine->box2d_world->SetDebugDraw( engine->physics_debug_draw.get() );
	engine->physics_debug_draw->SetFlags(
		0
		| b2Draw::e_shapeBit			// shapes
		| b2Draw::e_jointBit			// joint connections
		| b2Draw::e_aabbBit				// axis aligned bounding boxes
		| b2Draw::e_pairBit				// broad-phase pairs
		| b2Draw::e_centerOfMassBit		// center of mass frame
	);

#ifdef _DEBUG
	engine->render->show_extra_debug = true;
#else
	engine->render->show_extra_debug = false;
#endif
}

// called ONCE, as the engine is starting up

void w_engine::init()
{
	// make sure we are only calling this function ONE time per instance
	assert( time == nullptr );

	//stbi_set_flip_vertically_on_load( 1 );

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
	stats = std::make_unique<w_render_stats>();

	fs->init();
}

void w_engine::deinit()
{
	asset_definition_file_cache = nullptr;

	// this block seems unnecessary and I believe can be the source of a
	// shutdown crash if enabled. tread lightly.
	/*
	time = nullptr;
	asset_cache = nullptr;
	layer_mgr = nullptr;
	window = nullptr;
	render = nullptr;
	input = nullptr;
	ui = nullptr;
	fs = nullptr;
	opengl = nullptr;
	config_vars = nullptr;
	random = nullptr;
	*/
}

void w_engine::draw()
{
	// If engine is paused, draw a border around the screen for visibility

	if( is_paused() )
	{
		w_vec2 v0, v1, v2, v3;
		float w, h;

		{
			scoped_render_state;

			render_state.z = zdepth_topmost;

			w = ui_w;
			h = ui_h;

			v0 = w_vec2::zero;
			v1 = w_vec2( w - 1, 0.0f );
			v2 = w_vec2( w - 1, h - 1 );
			v3 = w_vec2( 0.0f, h - 1 );

			render_state.color = w_color::black;
			w_render::draw_line( v0, v1 );
			w_render::draw_line( v1, v2 );
			w_render::draw_line( v2, v3 );
			w_render::draw_line( v3, v0 );

			v0 = w_vec2( 1, 1 );
			v1 = w_vec2( w - 2, 1.0f );
			v2 = w_vec2( w - 2, h - 2 );
			v3 = w_vec2( 1.0f, h - 2 );

			render_state.color = w_color::orange;
			w_render::draw_line( v0, v1 );
			w_render::draw_line( v1, v2 );
			w_render::draw_line( v2, v3 );
			w_render::draw_line( v3, v0 );

			v0 = w_vec2( 2.0f, 2.0f );
			v1 = w_vec2( w - 3, 2.0f );
			v2 = w_vec2( w - 3, h - 3 );
			v3 = w_vec2( 2.0f, h - 3 );

			render_state.color = w_color::black;
			w_render::draw_line( v0, v1 );
			w_render::draw_line( v1, v2 );
			w_render::draw_line( v2, v3 );
			w_render::draw_line( v3, v0 );
		}
	}
}

void w_engine::update()
{
	layer_mgr->update();

	for( const auto& [xxx, asset] : asset_cache->cache )
	{
		asset->update();
	}
}

void w_engine::toggle_pause()
{
	pause_state.toggle = !pause_state.toggle;
}

void w_engine::pause()
{
	pause_state.ref_count++;
}

void w_engine::resume()
{
	pause_state.ref_count--;
}

bool w_engine::is_paused()
{
	return pause_state.toggle || pause_state.ref_count > 0;
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
	auto file = engine->fs->load_text_file( filename );

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

// iterate through all the cached asset_definition_files and look for any
// asset_definitions that have assets we need to precache
//
// things like texture files, sound files, etc.

void w_engine::precache_asset_resources( int pass )
{
	for( auto& iter : engine->asset_definition_file_cache->cache )
	{
		iter.precache_asset_resources( pass );
	}

	log( "Pass: {} / {} total assets precached", pass, f_commas( static_cast<float>( engine->asset_cache->cache.size() ) ) );
}

// loops through all threads we have a handle for and waits until they finish
// executing. the list is then cleared.

void w_engine::wait_for_thread_pool_to_finish()
{
	log_verbose( "Waiting for {} threads to finish.", threads.size() );

	for( auto& thread : threads )
	{
		thread.wait();
	}

	threads.clear();
}

bool w_engine::on_input_pressed( const w_input_event* evt )
{
	switch( evt->input_id )
	{
		// toggle engine pause
		case input_id::key_pause:
		{
			toggle_pause();

			return true;
		}

		// slow down game clock
		case input_id::key_left_bracket:
		{
			set_time_dilation( engine->input->is_shift_down() ? 1.0f : time->dilation - 0.1f );
			return true;
		}

		// speed up game clock
		case input_id::key_right_bracket:
		{
			set_time_dilation( engine->input->is_shift_down() ? 5.0f : time->dilation + 0.1f );
			return true;
		}

	#ifndef _FINALRELEASE
		// frame debugger
		case input_id::key_f10:
		{
			engine->render->single_frame_debugger = true;
			log_div();
			log( "-- Single Frame Debugger" );
			log_div();

			return true;
		}

		// toggle debug physics drawing
		case input_id::key_f5:
		{
			engine->render->show_extra_debug = !engine->render->show_extra_debug;

			return true;
		}
	#endif

		// toggle full screen
		case input_id::key_f11:
		{
			window->toggle_fullscreen();

			return true;
		}

		case input_id::key_enter:
		{
			if( engine->input->is_alt_down() )
			{
				window->toggle_fullscreen();
			}

			return true;
		}

		// toggle esc menu
		case input_id::key_esc:
		{
			auto layer_ptr = layer_mgr->get_top();
			if( typeid( *layer_ptr ) == typeid( layer_esc_menu ) )
			{
				layer_mgr->pop();
			}
			else
			{
				layer_mgr->push<layer_esc_menu>();
			}

			return true;
		}

		case input_id::key_f8:
		{
			render->show_stats = true;

			return true;
		}
	}

	return false;
}

bool w_engine::on_input_released( const w_input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_f8:
		{
			render->show_stats = false;

			return true;
		}
	}

	return false;
}

void w_engine::process_collision_queue()
{
	// begin contact

	for( auto& iter : begin_contact_queue )
	{
		iter.entity_a->on_collision_begin( iter, iter.entity_b );
		iter.entity_b->on_collision_begin( iter, iter.entity_a );
	}

	begin_contact_queue.clear();

	// end contact

	for( auto& iter : end_contact_queue )
	{
		iter.entity_a->on_collision_end( iter, iter.entity_b );
		iter.entity_b->on_collision_end( iter, iter.entity_a );
	}

	end_contact_queue.clear();
}

void w_engine::set_time_dilation( float dilation )
{
	time->dilation = glm::clamp( dilation, 0.1f, 5.0f );
	adjust_assets_for_time_dilation();
}

// loop through every cached sound and music file and if they are currently
// playing, set their pitch to match the current tile dilation.

void w_engine::adjust_assets_for_time_dilation()
{
	for( auto& iter : asset_cache->cache )
	{
		auto asset = iter.second.get();
		asset->adjust_for_time_dilation();
	}
}