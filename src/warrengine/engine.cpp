
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void engine::launch( int argc, char* argv [] )
{
#if defined(_FINAL_RELEASE)
	// in final release, we don't want to bother the user with the visual
	// clutter of the console window
	FreeConsole();
#endif

	// get the log file running so we can immediately start writing into it

	g_logfile = std::make_unique<log_file>();
	g_logfile->init( g_base_game->internal_name );
	log( "Logging started" );

	log( "Creating engine" );
	g_engine = std::make_unique<engine>();

	log( "Initializing engine" );
	g_engine->init();

	log( "Current Path : {}", std::filesystem::current_path().generic_string() );

	g_ui = std::make_unique<war::ui_mgr>();

	engine::launch_command_line( argc, argv );

	log( "Creating main window" );
	g_engine->window.init();

	log( "Initializing OpenGL" );
	g_engine->render_api.init();

	engine::launch_precache();

	log( "Initializing Box2D physics" );
	g_engine->new_physics_world();

	engine::launch_init_frame_buffers();
	engine::launch_init_engine_assets();

	log( "Initializing renderer" );
	g_engine->renderer.init();

	log( "Initializing game" );
	g_base_game->init();

	log( "Initializing input" );
	g_engine->input.init();

	g_engine->is_running = true;
	g_engine->time.init();
	g_engine->stats.init();

	log( "Running..." );

	g_engine->main_loop();

	log( "Shutting down..." );
	g_engine->shutdown();
}

void engine::launch_init_engine_assets()
{
	// used for solid drawing
	g_engine->tex_white = g_engine->find_asset<texture_asset>( "engine_white" );

	// there's a simple pixel font that always lives inside of engine so there
	// is always a font available, regardless of ui theme settings.
	g_engine->pixel_font = g_engine->find_asset<font_asset>( "engine_pixel_font" );
}

void engine::launch_init_frame_buffers()
{
	vec2 viewport_sz = { viewport_w, viewport_h };

	g_engine->frame_buffer = std::make_unique<opengl_framebuffer>( "game" );
	{
		g_engine->frame_buffer->add_color_attachment( viewport_sz, g_engine->window.window_clear_color );	// color
		g_engine->frame_buffer->add_color_attachment( viewport_sz );										// glow
		g_engine->frame_buffer->add_color_attachment( viewport_sz );										// pick_ids
		g_engine->frame_buffer->add_depth_attachment( viewport_sz );										// depth/stencil
		g_engine->frame_buffer->finalize();
	}

	g_engine->blur_frame_buffer = std::make_unique<opengl_framebuffer>( "blur" );
	{
		g_engine->blur_frame_buffer->add_color_attachment( viewport_sz );
		g_engine->blur_frame_buffer->finalize();
	}

	g_engine->composite_frame_buffer = std::make_unique<opengl_framebuffer>( "composite" );
	{
		g_engine->composite_frame_buffer->add_color_attachment( viewport_sz );
		g_engine->composite_frame_buffer->finalize();
	}
}

void engine::launch_command_line( int argc, char* argv [] )
{
	for( auto x = 1 ; x < argc ; ++x )
	{
		std::string arg = argv[ x ];

		if( arg == "-verbose" )
		{
			g_engine->cmdline.verbose = true;
			log_verbose( "cmdline : \"{}\" : Verbose logging enabled.", arg );
		}

		if( arg == "-nobatch" )
		{
			g_engine->cmdline.nobatch = true;
			log_verbose( "cmdline : \"{}\" : Batch rendering disabled.", arg );
		}
	}
}

void engine::launch_precache()
{
	log( "Caching asset definitions (*.asset_def)..." );
	g_engine->cache_asset_definition_files( "data/warrengine" );
	g_engine->cache_asset_definition_files( std::format( "data/{}", g_base_game->internal_name ) );

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
	g_engine->precache_asset_resources( 0 );
	g_engine->precache_asset_resources( 1 );

	// parse INI files after the preprocess pass so they can use
	// preprocessor symbols

	log( "Caching configuration (*.ini)..." );
	g_engine->parse_config_files( "data/warrengine" );
	g_engine->parse_config_files( std::format( "data/{}", g_base_game->internal_name ) );

	// put the k/v pairs from the INI files into the global symbol table so
	// they can be referenced by assets in the asset_def files

	for( const auto& [key, value] : g_engine->config_vars.kv )
	{
		g_engine->_symbol_to_value[ key ] = value;
	}

	engine::launch_apply_config_settings();

	g_engine->precache_asset_resources( 2 );
	g_engine->precache_asset_resources( 3 );
	g_engine->precache_asset_resources( 4 );

	g_engine->asset_def_file_cache.clear();
}

void engine::launch_apply_config_settings()
{
	tokenizer tok;

	tok.init( g_engine->config_vars.find_value_or( "viewport_res", "320x240" ), "x" );
	viewport_w = text_parser::float_from_str( tok.tokens[ 0 ] );
	viewport_h = text_parser::float_from_str( tok.tokens[ 1 ] );
	g_engine->_symbol_to_value[ "viewport_w" ] = std::format( "{}", viewport_w );
	g_engine->_symbol_to_value[ "viewport_h" ] = std::format( "{}", viewport_h );
	g_engine->_symbol_to_value[ "viewport_hw" ] = std::format( "{}", viewport_hw );
	g_engine->_symbol_to_value[ "viewport_hh" ] = std::format( "{}", viewport_hh );
	log( "V Window Res: {}x{}", (int)viewport_w, (int)viewport_h );

	tok.init( g_engine->config_vars.find_value_or( "ui_res", "640x480" ), "x" );
	ui_w = text_parser::float_from_str( tok.tokens[ 0 ] );
	ui_h = text_parser::float_from_str( tok.tokens[ 1 ] );
	g_engine->_symbol_to_value[ "ui_w" ] = std::format( "{}", ui_w );
	g_engine->_symbol_to_value[ "ui_h" ] = std::format( "{}", ui_h );
	g_engine->_symbol_to_value[ "ui_hw" ] = std::format( "{}", ui_hw );
	g_engine->_symbol_to_value[ "ui_hh" ] = std::format( "{}", ui_hh );
	log( "UI Window Res: {}x{}", (int)ui_w, (int)ui_h );


	g_engine->renderer.init_set_up_default_palette();
	render::palette = *( g_engine->find_asset<palette_asset>( g_engine->config_vars.find_value_or( "palette_tag", "pal_default" ) ) );

	rect rc = g_engine->window.compute_max_window_size_for_desktop();
	glfwSetWindowPos( g_engine->window.glfw_window, (int)( rc.x ), (int)( rc.y ) );
	glfwSetWindowSize( g_engine->window.glfw_window, (int)( rc.w ), (int)( rc.h ) );
	glfwSetWindowAspectRatio( g_engine->window.glfw_window,
		100,
		(int)( ( viewport_h / viewport_w ) * 100 ) );

	bool vsync = text_parser::bool_from_str( g_engine->config_vars.find_value_or( "v_sync", "false" ) );
	log( "VSync: {}", vsync ? "true" : "false" );
	glfwSwapInterval( vsync ? 1 : 0 );
	g_engine->window.set_title( g_engine->config_vars.find_value_or( "app_title", "WarrEngine" ) );
	glfwSetWindowAttrib( g_engine->window.glfw_window, GLFW_FLOATING, text_parser::bool_from_str( g_engine->config_vars.find_value_or( "always_on_top", "false" ) ) );
	g_engine->window.viewport_clear_color = text_parser::color_from_str( g_engine->config_vars.find_value_or( "viewport_clear_color", "64,64,64" ) );
	g_engine->window.window_clear_color = text_parser::color_from_str( g_engine->config_vars.find_value_or( "window_clear_color", "32,32,32" ) );
}

void engine::shutdown()
{
	// Clean up
	log( "Shutting down..." );

	log( "Shutting down scene manager" );

	// note : this needs to be done before the audio or windowing systems, to
	// give the scenes a chance to clean up first.
	scenes.clear_stack();

	log( "Shutting down window" );
	window.deinit();

	log( "Shutting down GLFW" );
	glfwTerminate();

	log( "Shutting down OpenGL" );
	for( auto& [name, shader] : g_engine->render_api.shaders )
	{
		glDeleteProgram( shader.gl_id );
	}

	log( "Shutting down input" );
	input.deinit();

	log( "Shutting down engine" );
	deinit();

	// Do this last so we can log right up until the last moment
	g_logfile->time_stamp( "Ended" );
	g_logfile->deinit();
}

void engine::main_loop()
{
	while( is_running and !glfwWindowShouldClose( window.glfw_window ) )
	{
		// update core engine stuff - time, timers, etc

		time.update();
		g_ui->reset();

		// whatever remaining ms are left in time.fts_accum_ms should be passed
		// to the render functions for interpolation/prediction
		//
		// it is passed a percentage for easier use : 0.f-1.f

		g_engine->renderer.frame_interpolate_pct = time.fts_accum_ms / (float)fixed_time_step::ms_per_step;

		if( time.fts_accum_ms >= fixed_time_step::ms_per_step )
		{
			// walk through each pending fixed time step, one at a time

			for( ; time.fts_accum_ms >= fixed_time_step::ms_per_step ; time.fts_accum_ms -= fixed_time_step::ms_per_step )
			{
				post_process.film_grain_amount += 0.01f;

				box2d.world->Step( fixed_time_step::per_second( 1.0f ), b2d_velocity_iterations, b2d_pos_iterations );

				input.queue_motion();

				scenes.pre_update();
				scenes.update();
				scenes.post_update();

				dispatch_collision_queue();

				g_base_game->update();
			}

			g_engine->stats.update();

			g_engine->render_api.set_uniform( "u_current_time", (float)time.now() / 1000.f );
			g_engine->render_api.set_uniform( "u_film_grain_amount", post_process.film_grain_amount );
		}

		// ----------------------------------------------------------------------------
		// draw the scene to the engine frame buffer
		//
		// this draws several versions of the scene at once:
		//
		// 1. the scene as normal
		// 2. same as #1 but only contains the brightest pixels (used for glow effects)
		// 3. entity pick ids

		input.queue_presses();
		input.dispatch_event_queue();

		g_engine->render_api.clear_depth_buffer();
		frame_buffer->bind();
		g_engine->render_api.shaders[ "base_pass" ].bind();
		g_engine->renderer.begin_frame();
		{
			// ----------------------------------------------------------------------------
			// render the game frame
			// ----------------------------------------------------------------------------

			g_engine->render_api.set_projection_matrix();

			{
				scoped_opengl;

				// scenes and entities
				scenes.draw();
			}

			// engine specific things, like pause borders
			draw();
			g_engine->renderer.dynamic_batches.flush_and_reset();
		}
		g_engine->renderer.end_frame();
		frame_buffer->unbind();

		do_draw_finished_frame();

		debug_draw_buffers();

		glfwSwapBuffers( window.glfw_window );
		glfwPollEvents();
	}
}

void engine::do_draw_finished_frame()
{
	static auto blur_shader_name = std::format( "blur_{}", g_engine->config_vars.find_value_or( "blur_method", "gaussian" ) );
	static auto blur_kernel_size = war::text_parser::float_from_str( g_engine->config_vars.find_value_or( "blur_kernel_size", "5" ) );

	// ----------------------------------------------------------------------------
	// the engine frame buffer now contains the color texture and the glow
	// texture. draw the glow texture into the blur frame buffer, using the
	// blur shader.
	// ----------------------------------------------------------------------------

	g_engine->render_api.set_view_matrix_identity_no_camera();
	blur_frame_buffer->bind();

	g_engine->render_api.shaders[ blur_shader_name ].bind();

	g_engine->render_api.set_uniform( "u_kernel_size", blur_kernel_size );

	g_engine->render_api.set_uniform( "u_viewport_w", viewport_w );
	g_engine->render_api.set_uniform( "u_viewport_h", viewport_h );

	render::draw_quad( frame_buffer->color_attachments[ framebuffer::glow ].texture, rect( 0.f, 0.f, viewport_w, viewport_h ) );
	g_engine->renderer.dynamic_batches.flush_and_reset_internal();
	blur_frame_buffer->unbind();

	{
		composite_frame_buffer->bind();

		// ----------------------------------------------------------------------------
		// draw the base frame buffer into the compositing frame buffer
		// ----------------------------------------------------------------------------

		{
			scoped_render_state;

			g_engine->render_api.shaders[ "compositing_pass" ].bind();

			render::draw_quad( frame_buffer->color_attachments[ 0 ].texture, rect( 0.f, 0.f, viewport_w, viewport_h ) );
			g_engine->renderer.dynamic_batches.flush_and_reset_internal();
		}

		// ----------------------------------------------------------------------------
		// overlay the glow frame buffer on top of the compositing frame buffer
		// ----------------------------------------------------------------------------

		{
			g_engine->render_api.shaders[ "simple" ].bind();
			g_engine->render_api.set_blend( opengl_blend::glow );

			render::draw_quad( blur_frame_buffer->color_attachments[ 0 ].texture, rect( 0.f, 0.f, viewport_w, viewport_h ) );
			g_engine->renderer.dynamic_batches.flush_and_reset_internal();

			g_engine->render_api.set_blend( opengl_blend::alpha );
		}

		composite_frame_buffer->unbind();
	}

	// ----------------------------------------------------------------------------
	// draw the compositing frame buffer to the default/final frame buffer
	// applying any screen based post process effects.
	// ----------------------------------------------------------------------------

	{
		g_engine->render_api.shaders[ "final_pass" ].bind();
		g_engine->render_api.set_view_matrix_identity_no_camera();

		// reset the viewport to the size of the actual window size
		glViewport(
			(int)window.viewport_pos_sz.x,
			(int)window.viewport_pos_sz.y,
			(int)window.viewport_pos_sz.w,
			(int)window.viewport_pos_sz.h
		);

		render::draw_quad( composite_frame_buffer->color_attachments[ 0 ].texture, rect( 0.f, 0.f, viewport_w, viewport_h ) );
		g_engine->renderer.dynamic_batches.flush_and_reset_internal();
	}
}

// checks if 'symbol' exists in the map

bool engine::is_symbol_in_map( std::string_view symbol )
{
	return _symbol_to_value.count( std::string( symbol ) ) > 0;
}

// returns a string containing the value stored for 'symbol'
std::optional<std::string> engine::find_string_from_symbol( std::string_view symbol )
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

int engine::find_int_from_symbol( std::string_view symbol, int def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = std::format( "{}", def_value );
	}

	return string_util::to_int( std::string( *sval ) );
}

float engine::find_float_from_symbol( std::string_view symbol, float def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = std::format( "{}", def_value );
	}

	return string_util::to_float( *sval );
}

color engine::find_color_from_symbol( std::string_view symbol, const color& def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = std::format( "{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a );
	}

	return color( *sval );
}

range<float> engine::find_range_from_symbol( std::string_view symbol, const range<float>& def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = std::format( "{},{}", def_value.start, def_value.end );
	}

	return range<float>( *sval );
}

vec2 engine::find_vec2_from_symbol( std::string_view symbol, const vec2& def_value )
{
	auto sval = find_string_from_symbol( symbol );

	if( !sval.has_value() )
	{
		sval = std::format( "{},{}", def_value.x, def_value.y );
	}

	return vec2( *sval );
}

void engine::new_physics_world()
{
	// box2d

	g_engine->box2d.world = std::make_unique<b2World>( b2Vec2( 0.f, b2d_gravity_default ) );
	g_engine->box2d.listener = std::make_unique<box2d_physics::contact_listener>();
	g_engine->box2d.world->SetContactListener( g_engine->box2d.listener.get() );

	g_engine->box2d.debug_draw = std::make_unique<war::box2d_physics::debug_draw>();
	g_engine->box2d.world->SetDebugDraw( g_engine->box2d.debug_draw.get() );
	g_engine->box2d.debug_draw->SetFlags(
		0
		| b2Draw::e_shapeBit			// shapes
		| b2Draw::e_jointBit			// joint connections
		| b2Draw::e_aabbBit				// axis aligned bounding boxes
		| b2Draw::e_pairBit				// broad-phase pairs
		| b2Draw::e_centerOfMassBit		// center of mass frame
	);

	// simple collision

	g_engine->simple_collision.world = std::make_unique<simple_collision::world>( vec2( 0.f, simple_collision_gravity_default ) );
}

// called ONCE, as the engine is starting up

void engine::init()
{
	random::seed();
	file_system::init();
}

void engine::deinit()
{
	// NOTE : let the OS handle cleaning up. trying to do it manually can cause
	// weird crashes.
}

void engine::draw()
{
	// If engine is paused, draw a border around the screen for visibility

	if( is_paused() )
	{
		{
			scoped_render_state;

			render::state->z = zdepth_topmost;

			rect rc( 0.f, 0.f, ui_w, ui_h );

			render::state->color = make_color( pal::darker );
			render::draw_line_loop( rc );

			rc.shrink( 1.0f );
			render::state->color = make_color( pal::middle );
			render::draw_line_loop( rc );

			rc.shrink( 1.0f );
			render::state->color = make_color( pal::brighter );
			render::draw_line_loop( rc );
		}
	}
}

void engine::toggle_pause()
{
	toggle_bool( pause_state.toggle );
}

void engine::pause()
{
	pause_state.ref_count++;
}

void engine::resume()
{
	pause_state.ref_count--;
}

bool engine::is_paused()
{
	return pause_state.toggle or pause_state.ref_count > 0;
}

// loads and caches every "*.asset_def" file it sees in the "asset_def" folder

void engine::cache_asset_definition_files( std::string_view folder_name )
{
	std::vector<std::string> filenames;
	file_system::scan_folder_for_ext( &filenames, std::format( "{}", folder_name ), ".asset_def" );

	for( const auto& iter : filenames )
	{
		asset_file_definition asset_def_file = {};
		asset_def_file.original_filename = iter;

		if( asset_def_file.create_internals() )
		{
			asset_def_file_cache.push_back( asset_def_file );
		}
	}
}

void engine::parse_config_files( std::string_view folder_name )
{
	std::vector<std::string> filenames;
	file_system::scan_folder_for_ext( &filenames, std::format( "{}", folder_name ), ".ini" );

	for( const auto& iter : filenames )
	{
		g_engine->parse_config_file( iter );
	}
}

void engine::parse_config_file( std::string_view filename )
{
	auto file = file_system::load_text_file( filename );

	for( const auto& line : file->lines )
	{
		if( line.substr( 0, 1 ) == "\"" )
		{
			tokenizer tok_kv( line, "\"" );

			auto key = tok_kv.get_next_token();
			tok_kv.get_next_token(); // skip blank
			auto value = tok_kv.get_next_token();

			if( key.has_value() and value.has_value() )
			{
				config_vars.kv.insert_or_assign( std::string( *key ), std::string( *value ) );
			}
		}
	}
}

// iterate through all the cached asset_definition_files and look for any
// asset_definitions that have assets we need to precache
//
// things like texture files, sound files, etc.

void engine::precache_asset_resources( int pass )
{
	for( auto& iter : asset_def_file_cache )
	{
		iter.precache_asset_resources( pass );
	}

	log( "Pass: {} / {} total assets precached", pass, f_commas( (float)( g_engine->asset_cache.cache.size() ) ) );
}

// loops through all threads we have a handle for and waits until they finish
// executing. the list is then cleared.

void engine::wait_for_thread_pool_to_finish()
{
	log_verbose( "Waiting for {} threads to finish.", threads.size() );

	for( auto& thread : threads )
	{
		thread.wait();
	}

	threads.clear();
}

bool engine::on_input_motion( const input_event* evt )
{
	auto cam_transform = scenes.get_transform();

	switch( evt->input_id )
	{
		case input_id::mouse:
		{
			// camera control
			if( g_engine->input.is_button_held( input_id::mouse_button_middle ) )
			{
				if( evt->control_down )
				{
					cam_transform->add_angle( coord_system::window_to_viewport_vec( evt->delta ).x );
				}
				else if( evt->alt_down )
				{
					cam_transform->add_scale( coord_system::window_to_viewport_vec( evt->delta ).x * 0.01f );
				}
				else
				{
					vec2 delta = coord_system::window_to_world_vec( evt->delta );
					cam_transform->add_pos( delta );
				}

				return true;
			}
		}
		break;

		case input_id::mouse_wheel:
		{
			cam_transform->add_scale( coord_system::window_to_viewport_vec( evt->delta ).y * 0.25f );

			return true;
		}
		break;
	}

	return false;
}

bool engine::on_input_pressed( const input_event* evt )
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
			set_time_dilation( g_engine->input.is_shift_down() ? 1.f : time.dilation - 0.1f );
			return true;
		}

		// speed up game clock
		case input_id::key_right_bracket:
		{
			set_time_dilation( g_engine->input.is_shift_down() ? 5.f : time.dilation + 0.1f );
			return true;
		}

	#ifndef _FINAL_RELEASE
		// frame debugger
		case input_id::key_f10:
		{
			g_engine->renderer.debug.single_frame_log = true;
			log_div();
			log( "-- Single Frame Debugger" );
			log_div();

			return true;
		}

		// toggle debug physics drawing
		case input_id::key_f5:
		{
			toggle_bool( g_engine->renderer.debug.draw_debug_info );
			return true;
		}
	#endif

		// post process tweaker
		case input_id::key_f4:
		{
			g_engine->scenes.push<scene_post_process>();

			return true;
		}

		// toggle full screen
		case input_id::key_f11:
		{
			window.toggle_fullscreen();

			return true;
		}

		case input_id::key_enter:
		{
			if( g_engine->input.is_alt_down() )
			{
				window.toggle_fullscreen();
			}

			return true;
		}

		// toggle esc menu
		case input_id::key_esc:
		{
			auto scene_ptr = scenes.get_top();

			// if there are UI controls in the current scene that are expanded,
			// then hitting ESC will force them closed instead of toggling the
			// ESC menu.

			if( scene_ptr->ui_expanded_tag_begin != hash_none )
			{
				scene_ptr->force_close_expanded_controls();
				return true;
			}

			// if we've reached this point, toggle the ESC menu as normal.

			if( typeid( *scene_ptr ) == typeid( scene_esc_menu ) )
			{
				scenes.pop();
			}
			else
			{
				scenes.push<scene_esc_menu>();
			}

			return true;
		}

		case input_id::key_f8:
		{
			stats.draw_verbose = true;
			return true;
		}
	}

	return false;
}

bool engine::on_input_released( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_f8:
		{
			stats.draw_verbose = false;
			return true;
		}
	}

	return false;
}

void engine::dispatch_collision_queue()
{
	dispatch_box2d_collisions();
	dispatch_simple_collisions();
}

void engine::dispatch_box2d_collisions()
{
	// begin contact

	for( auto& iter : box2d.begin_contact_queue )
	{
		iter.entity_a->on_box2d_collision_begin( iter, iter.entity_b );
		iter.entity_b->on_box2d_collision_begin( iter, iter.entity_a );
	}

	box2d.begin_contact_queue.clear();

	// end contact

	for( auto& iter : box2d.end_contact_queue )
	{
		// sometimes we get garbage entities/fixtures in this iterator when
		// shutting down the world. i think checking the restitution value like
		// this is hacky and feels bad but it seems to work. i don't know what a
		// proper solution looks like yet.

		if( iter.fixture_a->GetRestitution() < 0.f or iter.fixture_b->GetRestitution() < 0.f )
		{
			continue;
		}

		iter.entity_a->on_box2d_collision_end( iter, iter.entity_b );
		iter.entity_b->on_box2d_collision_end( iter, iter.entity_a );
	}

	box2d.end_contact_queue.clear();
}

void engine::dispatch_simple_collisions()
{
	for( auto& iter : simple_collision.queue )
	{
		iter.entity_a->on_simple_collision( iter, iter.entity_b );
	}

	simple_collision.queue.clear();
}

void engine::set_time_dilation( float dilation )
{
	time.dilation = glm::clamp( dilation, 0.1f, 5.f );

	// give all assets a chance to respond to the time dilation change

	for( auto& [name, asset] : asset_cache.cache )
	{
		asset->adjust_for_time_dilation();
	}
}

void engine::show_msg_box( std::string_view msg )
{
	msg_box.msg = msg;
	g_engine->scenes.push<scene_msg_box>();
}

void engine::debug_draw_buffers()
{
#if 0
	// ---------------------------------------------------------------------------
	// debug helper
	//
	// draw all the color attachments in quads at the bottom of the viewport
	// ----------------------------------------------------------------------------

	g_engine->render_api.shaders[ "simple" ].bind();

	auto num_color_attachments = frame_buffer->color_attachments.size();
	float scale_factor = 1.f / (float)num_color_attachments;
	float w = viewport_w * scale_factor;
	float h = viewport_h * scale_factor;
	rect rc = { 0.f, viewport_h - h, w, h };

	std::array<const char*, framebuffer::max> names = { "color", "glow", "pick", "blur", "comp", "final" };

	{
		scoped_render_state;

		for( int x = 0 ; x < num_color_attachments ; ++x )
		{
			render::draw_quad( frame_buffer->color_attachments[ x ].texture, rc );
			render::draw_string( names[ x ], { rc.x, rc.y } );
			rc.x += w;
		}

		g_engine->renderer.dynamic_batches.flush_and_reset_internal();
	}

#endif
}

}
