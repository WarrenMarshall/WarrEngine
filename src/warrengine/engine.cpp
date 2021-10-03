
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Engine::launch( int32_t argc, char* argv [] )
{
#if defined(_FINAL_RELEASE)
	// in final release, we don't want to bother the user with the visual
	// clutter of the console window
	FreeConsole();
#endif

	// get the log file running so we can immediately start writing into it

	g_logfile = std::make_unique<Log_File>();
	g_logfile->init( g_base_game->internal_name );
	g_logfile->time_stamp( "Started" );

	log( "CPU Cache Line Size : {}", OS_Util::get_cpu_cache_line_sz() );

	log( "Creating engine" );
	g_engine = std::make_unique<Engine>();

	log( "Initializing engine" );
	g_engine->init();

	log( "Current Path : {}", std::filesystem::current_path().generic_string() );

	g_ui = std::make_unique<war::UI_Mgr>();

	Engine::parse_command_line( argc, argv );

	log( "Creating main window" );
	g_engine->window.init();

	log( "Initializing OpenGL" );
	g_engine->opengl_mgr.init();

	Engine::precache();

	log( "Initializing Box2D physics" );
	g_engine->new_physics_world();

	Vec2 viewport_sz = { viewport_w, viewport_h };

	g_engine->frame_buffer.init( "game" );
	{
		g_engine->frame_buffer.add_color_attachment(viewport_sz, g_engine->window.window_clear_color);	// color
		g_engine->frame_buffer.add_color_attachment(viewport_sz);										// glow
		g_engine->frame_buffer.add_color_attachment(viewport_sz);										// pick_ids
		g_engine->frame_buffer.add_depth_attachment(viewport_sz);										// depth/stencil
		g_engine->frame_buffer.finalize();
	}

	g_engine->blur_frame_buffer.init( "blur" );
	{
		g_engine->blur_frame_buffer.add_color_attachment( viewport_sz );
		g_engine->blur_frame_buffer.finalize();
	}

	g_engine->composite_frame_buffer.init( "composite" );
	{
		g_engine->composite_frame_buffer.add_color_attachment( viewport_sz );
		g_engine->composite_frame_buffer.finalize();
	}

	// used for solid drawing
	g_engine->tex_white = g_engine->find_asset<Texture_Asset>( "engine_white" );

	// there's a simple pixel font that always lives inside of engine so there
	// is always a font available, regardless of ui theme settings.
	g_engine->pixel_font = g_engine->find_asset<Font_Asset>( "engine_pixel_font" );

	log( "Initializing renderer" );
	g_engine->render.init();
	g_engine->render.tex_lut = g_engine->find_asset<Texture_Asset>( "tex_lut_default" );

	log( "Initializing game" );
	g_base_game->init();

	log( "Initializing input" );
	g_engine->input_mgr.init();

	g_engine->is_running = true;
	g_engine->clock.init();
	g_engine->stats.init();

	log( "Running..." );

	g_engine->main_loop();

	log( "Shutting down..." );
	g_engine->shutdown();
}

void Engine::parse_command_line( int32_t argc, char* argv [] )
{
	for( auto x = 1; x < argc; ++x )
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

void Engine::precache()
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

	Engine::apply_config_settings();

	g_engine->precache_asset_resources( 2 );
	g_engine->precache_asset_resources( 3 );
	g_engine->precache_asset_resources( 4 );

	g_engine->asset_def_file_cache.clear();
}

void Engine::apply_config_settings()
{
	Tokenizer tok;

	tok.init( g_engine->config_vars.find_value_or( "viewport_res", "320x240" ), "x" );
	viewport_w = Text_Parser::float_from_str( tok.tokens[ 0 ] );
	viewport_h = Text_Parser::float_from_str( tok.tokens[ 1 ] );
	g_engine->_symbol_to_value[ "viewport_w" ] = std::format( "{}", viewport_w );
	g_engine->_symbol_to_value[ "viewport_h" ] = std::format( "{}", viewport_h );
	g_engine->_symbol_to_value[ "viewport_hw" ] = std::format( "{}", viewport_hw );
	g_engine->_symbol_to_value[ "viewport_hh" ] = std::format( "{}", viewport_hh );
	log( "V Window Res: {}x{}", ( int32_t )viewport_w, ( int32_t )viewport_h );

	tok.init( g_engine->config_vars.find_value_or( "ui_res", "640x480" ), "x" );
	ui_w = Text_Parser::float_from_str( tok.tokens[ 0 ] );
	ui_h = Text_Parser::float_from_str( tok.tokens[ 1 ] );
	g_engine->_symbol_to_value[ "ui_w" ] = std::format( "{}", ui_w );
	g_engine->_symbol_to_value[ "ui_h" ] = std::format( "{}", ui_h );
	g_engine->_symbol_to_value[ "ui_hw" ] = std::format( "{}", ui_hw );
	g_engine->_symbol_to_value[ "ui_hh" ] = std::format( "{}", ui_hh );
	log( "UI Window Res: {}x{}", ( int32_t )ui_w, ( int32_t )ui_h );

	tok.init( g_engine->config_vars.find_value_or( "final_pixel_res", "320x240" ), "x" );
	final_pixel_w = Text_Parser::float_from_str( tok.tokens[ 0 ] );
	final_pixel_h = Text_Parser::float_from_str( tok.tokens[ 1 ] );

	g_engine->render.init_set_up_default_palette();
	Render::palette = *( g_engine->find_asset<Palette_Asset>( g_engine->config_vars.find_value_or( "palette_tag", "pal_default" ) ) );

	Rect rc = g_engine->window.compute_max_window_size_for_desktop();
	glfwSetWindowPos( g_engine->window.glfw_window, ( int32_t )( rc.x ), ( int32_t )( rc.y ) );
	glfwSetWindowSize( g_engine->window.glfw_window, ( int32_t )( rc.w ), ( int32_t )( rc.h ) );
	glfwSetWindowAspectRatio( g_engine->window.glfw_window,
		100,
		( int32_t )( ( viewport_h / viewport_w ) * 100 ) );

	bool vsync = Text_Parser::bool_from_str( g_engine->config_vars.find_value_or( "v_sync", "false" ) );
	log( "VSync: {}", vsync ? "true" : "false" );
	glfwSwapInterval( vsync ? 1 : 0 );
	g_engine->window.set_title( g_engine->config_vars.find_value_or( "app_title", "WarrEngine" ) );
	glfwSetWindowAttrib( g_engine->window.glfw_window, GLFW_FLOATING, Text_Parser::bool_from_str( g_engine->config_vars.find_value_or( "always_on_top", "false" ) ) );
	g_engine->window.viewport_clear_color = Text_Parser::color_from_str( g_engine->config_vars.find_value_or( "viewport_clear_color", "64,64,64" ) );
	g_engine->window.window_clear_color = Text_Parser::color_from_str( g_engine->config_vars.find_value_or( "window_clear_color", "32,32,32" ) );
}

void Engine::shutdown()
{
	// Clean up
	log( "Shutting down..." );

	log( "Shutting down scene manager" );

	// note : this needs to be done before the audio or windowing systems, to
	// give the scenes a chance to clean up first.
	scene_mgr.clear_stack();

	log( "Shutting down window" );
	window.deinit();

	log( "Shutting down GLFW" );
	glfwTerminate();

	log( "Shutting down OpenGL" );
	for( auto& [name, Shader] : g_engine->opengl_mgr.shaders )
	{
		glDeleteProgram( Shader.gl_id );
	}

	log( "Shutting down input" );
	input_mgr.deinit();

	log( "Shutting down engine" );
	deinit();

	// Do this last so we can log right up until the last moment
	g_logfile->time_stamp( "Ended" );
	g_logfile->deinit();
}

void Engine::main_loop()
{
	while( is_running and !glfwWindowShouldClose( window.glfw_window ) )
	{
		// update core engine stuff - time, timers, etc

		clock.update();
		g_ui->reset();

		// whatever remaining ms are left in time.fts_accum_ms should be passed
		// to the render functions for interpolation/prediction
		//
		// it is passed a percentage for easier use : 0.f-1.f

		g_engine->render.frame_interpolate_pct = clock.fts_accum_ms / ( float_t )fixed_time_step::ms_per_step;

		// if due for a fixed time step ...

		bool fixed_time_step_due = ( clock.fts_accum_ms >= fixed_time_step::ms_per_step );

		if( fixed_time_step_due )
		{
			int32_t num_time_steps = 0;

			while( clock.fts_accum_ms >= fixed_time_step::ms_per_step )
			{
				clock.fts_accum_ms -= fixed_time_step::ms_per_step;
				num_time_steps++;
			}

			post_process.film_grain_amount += 0.01f;

			input_mgr.queue_presses();
			input_mgr.queue_motion();

			input_mgr.dispatch_event_queue();

			box2d.world->Step( fixed_time_step::per_second( 1.f ) * num_time_steps, b2d_velocity_iterations, b2d_pos_iterations );

			scene_mgr.pre_update();
			scene_mgr.update();
			scene_mgr.post_update();

			g_base_game->update();

			g_engine->stats.update();

			g_engine->opengl_mgr.set_uniform_float( "u_current_time", ( float_t )clock.now() / 1000.f );
			g_engine->opengl_mgr.set_uniform_float( "u_film_grain_amount", post_process.film_grain_amount );
		}

		// ----------------------------------------------------------------------------
		// draw the scene to the engine frame buffer
		//
		// this draws several versions of the scene at once:
		//
		// 1. the scene as normal
		// 2. same as #1 but only contains the brightest pixels (used for glow effects)
		// 3. entity pick ids

		g_engine->opengl_mgr.clear_depth_buffer();
		frame_buffer.bind();
		g_engine->opengl_mgr.shaders[ "base_pass" ].bind();
		g_engine->render.begin_frame();
		{
			// ----------------------------------------------------------------------------
			// render the game frame
			// ----------------------------------------------------------------------------

			g_engine->opengl_mgr.set_projection_matrix();

			{
				scoped_opengl;

				// scenes and entities
				scene_mgr.draw();

			}

			// engine specific things, like pause borders
			draw();

			g_engine->render.dynamic_batches.flush_and_reset( e_draw_call::opaque );
			g_engine->render.dynamic_batches.flush_and_reset( e_draw_call::transparent );
		}
		g_engine->render.end_frame();
		frame_buffer.unbind();

		do_draw_finished_frame();

		debug_draw_buffers();

		glfwSwapBuffers( window.glfw_window );

		glfwPollEvents();
	}
}

void Engine::do_draw_finished_frame()
{
	static auto blur_shader_name = std::format( "blur_{}", g_engine->config_vars.find_value_or( "blur_method", "gaussian" ) );
	static auto blur_kernel_size = war::Text_Parser::float_from_str( g_engine->config_vars.find_value_or( "blur_kernel_size", "5" ) );

	// ----------------------------------------------------------------------------
	// the engine frame buffer now contains the color texture and the glow
	// texture. draw the glow texture into the blur frame buffer, using the
	// blur shader.
	// ----------------------------------------------------------------------------

	g_engine->opengl_mgr.set_view_matrix_identity_no_camera();
	blur_frame_buffer.bind();

	g_engine->opengl_mgr.shaders[ blur_shader_name ].bind();

	g_engine->opengl_mgr.set_uniform_float( "u_kernel_size", blur_kernel_size );

	g_engine->opengl_mgr.set_uniform_float( "u_viewport_w", viewport_w );
	g_engine->opengl_mgr.set_uniform_float( "u_viewport_h", viewport_h );
	g_engine->opengl_mgr.set_uniform_float( "u_final_pixel_w", final_pixel_w );
	g_engine->opengl_mgr.set_uniform_float( "u_final_pixel_h", final_pixel_h );

	Render::draw_quad( frame_buffer.color_attachments[ 1 /* glow color attachment */ ].texture, Rect( 0.f, 0.f, viewport_w, viewport_h ) );
	g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::opaque );
	g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::transparent );
	blur_frame_buffer.unbind();

	{
		composite_frame_buffer.bind();

		// ----------------------------------------------------------------------------
		// draw the base frame buffer into the compositing frame buffer
		// ----------------------------------------------------------------------------

		{
			scoped_render_state;

			g_engine->opengl_mgr.shaders[ "compositing_pass" ].bind();

			Render::draw_quad( frame_buffer.color_attachments[ 0 ].texture, Rect( 0.f, 0.f, viewport_w, viewport_h ) );
			g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::opaque );
			g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::transparent );
		}

		// ----------------------------------------------------------------------------
		// overlay the glow frame buffer on top of the compositing frame buffer
		// ----------------------------------------------------------------------------

		{
			g_engine->opengl_mgr.shaders[ "simple" ].bind();
			g_engine->opengl_mgr.set_blend( e_opengl_blend::glow );

			Render::draw_quad( blur_frame_buffer.color_attachments[ 0 ].texture, Rect( 0.f, 0.f, viewport_w, viewport_h ) );

			g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::opaque );
			g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::transparent );

			g_engine->opengl_mgr.set_blend( e_opengl_blend::alpha );
		}

		composite_frame_buffer.unbind();
	}

	// ----------------------------------------------------------------------------
	// draw the compositing frame buffer to the default/final frame buffer
	// applying any screen based post process effects.
	// ----------------------------------------------------------------------------

	{
		g_engine->opengl_mgr.shaders[ "final_pass" ].bind();
		g_engine->opengl_mgr.set_view_matrix_identity_no_camera();

		// reset the viewport to the size of the actual window size
		glViewport(
			( int32_t )window.viewport_pos_sz.x,
			( int32_t )window.viewport_pos_sz.y,
			( int32_t )window.viewport_pos_sz.w,
			( int32_t )window.viewport_pos_sz.h
		);

		Render::state->batch_render_target->assign_texture_slot_manual( composite_frame_buffer.color_attachments[ 0 ].texture );
		Render::state->batch_render_target->assign_texture_slot_manual( g_engine->render.tex_lut );

		Render::draw_quad( composite_frame_buffer.color_attachments[0].texture, Rect(0.f, 0.f, viewport_w, viewport_h));
		g_engine->render.dynamic_batches.flush_and_reset_internal(e_draw_call::opaque);
		g_engine->render.dynamic_batches.flush_and_reset_internal(e_draw_call::transparent);
	}
}

// checks if 'symbol' exists in the map

bool Engine::is_symbol_in_map(std::string_view symbol)
{
	return _symbol_to_value.count(std::string(symbol)) > 0;
}

// returns a string containing the value stored for 'symbol'
std::optional<std::string> Engine::find_string_from_symbol(std::string_view symbol)
{
	if (!is_symbol_in_map(symbol))
	{
		return std::nullopt;
	}

	return _symbol_to_value[std::string(symbol)];
}

// returns a value to the caller based on the contents of 'symbol'
//
// if no value is found for 'symbol', the default value is returned

bool Engine::find_bool_from_symbol(std::string_view symbol, bool def_value)
{
	auto sval = find_string_from_symbol(symbol);

	if (!sval.has_value())
	{
		sval = std::format("{}", def_value);
	}

	return String_Util::to_int(std::string(*sval));
}

int32_t Engine::find_int_from_symbol(std::string_view symbol, int32_t def_value)
{
	auto sval = find_string_from_symbol(symbol);

	if (!sval.has_value())
	{
		sval = std::format("{}", def_value);
	}

	return String_Util::to_int(std::string(*sval));
}

float_t Engine::find_float_from_symbol(std::string_view symbol, float_t def_value)
{
	auto sval = find_string_from_symbol(symbol);

	if (!sval.has_value())
	{
		sval = std::format("{}", def_value);
	}

	return String_Util::to_float(*sval);
}

Color Engine::find_color_from_symbol(std::string_view symbol, const Color& def_value)
{
	auto sval = find_string_from_symbol(symbol);

	if (!sval.has_value())
	{
		sval = std::format("{},{},{},{}", def_value.r, def_value.g, def_value.b, def_value.a);
	}

	return Color(*sval);
}

Range<float_t> Engine::find_range_from_symbol(std::string_view symbol, const Range<float_t>& def_value)
{
	auto sval = find_string_from_symbol(symbol);

	if (!sval.has_value())
	{
		sval = std::format("{},{}", def_value.start, def_value.end);
	}

	return Range<float_t>(*sval);
}

Vec2 Engine::find_vec2_from_symbol(std::string_view symbol, const Vec2& def_value)
{
	auto sval = find_string_from_symbol(symbol);

	if (!sval.has_value())
	{
		sval = std::format("{},{}", def_value.x, def_value.y);
	}

	return Vec2(*sval);
}

void Engine::new_physics_world()
{
	// box2d

	g_engine->box2d.world = std::make_unique<b2World>(b2Vec2(0.f, b2d_gravity_default));

	g_engine->box2d.listener = {};
	g_engine->box2d.world->SetContactListener(&g_engine->box2d.listener);

	g_engine->box2d.debug_draw = {};
	g_engine->box2d.world->SetDebugDraw(&g_engine->box2d.debug_draw);

	g_engine->box2d.debug_draw.SetFlags(
		0
		| b2Draw::e_shapeBit			// shapes
		| b2Draw::e_jointBit			// joint connections
		| b2Draw::e_aabbBit				// axis aligned bounding boxes
		| b2Draw::e_pairBit				// broad-phase pairs
		| b2Draw::e_centerOfMassBit		// center of mass frame
	);
}

// called ONCE, as the engine is starting up

void Engine::init()
{
	Random::seed();
	File_System::init();
}

void Engine::deinit()
{
	// NOTE : let the OS handle cleaning up. trying to do it manually can cause
	// weird crashes.

	// this exception is made because without it there's an address sanitizer
	// crash that comes from, I believe, the order in which SFML sound stuff
	// gets destroyed. controlling when the asset cache gets cleared like this
	// seems to dictate the destruction order enough that we avoid the crash
	// when the sf::SoundBuffer in each sound_asset tries to destroy itself.

	asset_cache.cache.clear();
}

void Engine::draw()
{
	// If engine is paused, draw a border around the screen for visibility

	if (is_paused())
	{
		{
			scoped_render_state;

			Render::state->z = zdepth_topmost;

			Rect rc(0.f, 0.f, ui_w, ui_h);

			Render::state->color = make_color(e_pal::darker);
			Render::draw_line_loop(rc);

			rc.shrink(1.f);
			Render::state->color = make_color(e_pal::middle);
			Render::draw_line_loop(rc);

			rc.shrink(1.f);
			Render::state->color = make_color(e_pal::brighter);
			Render::draw_line_loop(rc);
		}
	}
}

void Engine::toggle_pause()
{
	toggle_bool( pause_state.toggle );
}

void Engine::pause()
{
	pause_state.ref_count++;
}

void Engine::resume()
{
	pause_state.ref_count--;
}

bool Engine::is_paused()
{
	return pause_state.toggle + pause_state.ref_count;
}

// loads and caches every "*.asset_def" file it sees in the "asset_def" folder

void Engine::cache_asset_definition_files(std::string_view folder_name)
{
	std::vector<std::string> filenames;
	File_System::scan_folder_for_ext(&filenames, std::format("{}", folder_name), ".asset_def");

	for (const auto& iter : filenames)
	{
		Asset_File_Definition asset_def_file = {};
		asset_def_file.original_filename = iter;

		if (asset_def_file.create_internals())
		{
			asset_def_file_cache.push_back(asset_def_file);
		}
	}
}

void Engine::parse_config_files(std::string_view folder_name)
{
	std::vector<std::string> filenames;
	File_System::scan_folder_for_ext(&filenames, std::format("{}", folder_name), ".ini");

	for (const auto& iter : filenames)
	{
		g_engine->parse_config_file(iter);
	}
}

void Engine::parse_config_file(std::string_view filename)
{
	auto file = File_System::load_text_file(filename);

	for (const auto& line : file->lines)
	{
		if (line.substr(0, 1) == "\"")
		{
			Tokenizer tok_kv(line, "\"");

			auto key = tok_kv.get_next_token();
			tok_kv.get_next_token(); // skip blank
			auto value = tok_kv.get_next_token();

			if (key.has_value() and value.has_value())
			{
				config_vars.kv.insert_or_assign(std::string(*key), std::string(*value));
			}
		}
	}
}

// iterate through all the cached asset_definition_files and look for any
// asset_definitions that have assets we need to precache
//
// things like texture files, sound files, etc.

void Engine::precache_asset_resources(int32_t pass)
{
	for (auto& iter : asset_def_file_cache)
	{
		iter.precache_asset_resources(pass);
	}

	log("Pass: {} / {} total assets precached", pass, f_commas((float_t)(g_engine->asset_cache.cache.size())));
}

// loops through all threads we have a handle for and waits until they finish
// executing. the list is then cleared.

void Engine::wait_for_thread_pool_to_finish()
{
	log_verbose("Waiting for {} threads to finish.", threads.size());

	for (auto& thread : threads)
	{
		thread.wait();
	}

	threads.clear();
}

bool Engine::on_input_motion(const Input_Event* evt)
{
	auto cam_transform = scene_mgr.get_transform();

	switch (evt->input_id)
	{
		case e_input_id::mouse:
		{
			// camera control
			if (g_engine->input_mgr.is_button_held(e_input_id::mouse_button_middle))
			{
				if (evt->control_down)
				{
					cam_transform->add_angle(Coord_System::window_to_viewport_vec(evt->delta).x);
				}
				else if (evt->alt_down)
				{
					cam_transform->add_scale(Coord_System::window_to_viewport_vec(evt->delta).x * 0.01f);
				}
				else
				{
					Vec2 delta = Coord_System::window_to_world_vec(evt->delta);
					cam_transform->add_pos(delta);
				}

				return true;
			}
		}
		break;

		case e_input_id::mouse_wheel:
		{
			cam_transform->add_scale(Coord_System::window_to_viewport_vec(evt->delta).y * 0.25f);

			return true;
		}
		break;
	}

	return false;
}

bool Engine::on_input_pressed(const Input_Event* evt)
{
	switch (evt->input_id)
	{
		// toggle engine pause
		case e_input_id::key_pause:
		{
			toggle_pause();

			return true;
		}

		// slow down game clock
		case e_input_id::key_left_bracket:
		{
			set_time_dilation(g_engine->input_mgr.is_shift_down() ? 1.f : clock.dilation - 0.1f);
			return true;
		}

		// speed up game clock
		case e_input_id::key_right_bracket:
		{
			set_time_dilation(g_engine->input_mgr.is_shift_down() ? 5.f : clock.dilation + 0.1f);
			return true;
		}

	#ifndef _FINAL_RELEASE
		case e_input_id::key_f9:
		{
			g_engine->render.debug.entity_info_log = true;
			log_div();
			log("-- Entity Info");
			log_div();

			return true;
		}

		case e_input_id::key_f10:
		{
			g_engine->render.debug.single_frame_log = true;
			log_div();
			log("-- Single Frame Debugger");
			log_div();

			return true;
		}

		// toggle debug drawing
		case e_input_id::key_f5:
		{
			toggle_bool(g_engine->render.debug.draw_colliders);
			return true;
		}

		case e_input_id::key_f6:
		{
			toggle_bool( g_engine->render.debug.draw_spatial );
			return true;
		}
#endif

		// post process tweaker
		case e_input_id::key_f4:
		{
			g_engine->scene_mgr.push<Scene_Post_Process>();

			return true;
		}

		// toggle full screen
		case e_input_id::key_f11:
		{
			window.toggle_fullscreen();

			return true;
		}

		case e_input_id::key_enter:
		{
			if (g_engine->input_mgr.is_alt_down())
			{
				window.toggle_fullscreen();
			}

			return true;
		}

		// toggle esc menu
		case e_input_id::key_esc:
		{
			auto scene_ptr = scene_mgr.get_top();

			// if there are UI controls in the current scene that are expanded,
			// then hitting ESC will force them closed instead of toggling the
			// ESC menu.

			if (scene_ptr->ui_expanded_tag_begin != hash_none)
			{
				scene_ptr->force_close_expanded_controls();
				return true;
			}

			// if we've reached this point, toggle the ESC menu as normal.

			if (typeid(*scene_ptr) == typeid(Scene_Esc_Menu))
			{
				scene_mgr.pop();
			}
			else
			{
				scene_mgr.push<Scene_Esc_Menu>();
			}

			return true;
		}

		case e_input_id::key_f8:
		{
			stats.draw_verbose = true;
			return true;
		}
	}

	return false;
}

bool Engine::on_input_released(const Input_Event* evt)
{
	switch (evt->input_id)
	{
		case e_input_id::key_f8:
		{
			stats.draw_verbose = false;
			return true;
		}
	}

	return false;
}

void Engine::dispatch_collision_queue()
{
	dispatch_box2d_collisions();
}

void Engine::dispatch_box2d_collisions()
{
	// begin contact

	for (auto& iter : box2d.begin_contact_queue)
	{
		iter.entity_a->on_box2d_collision_begin(iter, iter.entity_b);
		iter.entity_b->on_box2d_collision_begin(iter, iter.entity_a);
	}

	box2d.begin_contact_queue.clear();

	// end contact

	for (auto& iter : box2d.end_contact_queue)
	{
		// #hack
		// sometimes we get garbage entities/fixtures in this iterator when
		// shutting down the world. i think checking the restitution value like
		// this is hacky and feels bad but it seems to work. i don't know what a
		// proper solution looks like yet.

		if (iter.fixture_a->GetRestitution() < 0.f or iter.fixture_b->GetRestitution() < 0.f)
		{
			continue;
		}

		iter.entity_a->on_box2d_collision_end(iter, iter.entity_b);
		iter.entity_b->on_box2d_collision_end(iter, iter.entity_a);
	}

	box2d.end_contact_queue.clear();
}

void Engine::set_time_dilation(float_t dilation)
{
	clock.dilation = glm::clamp(dilation, 0.1f, 5.f);

	// give all assets a chance to respond to the time dilation change

	for (auto& [name, Asset] : asset_cache.cache)
	{
		Asset->adjust_for_time_dilation();
	}
}

void Engine::show_msg_box(std::string_view msg)
{
	msg_box.msg = msg;
	g_engine->scene_mgr.push<Scene_Msg_Box>();
}

void Engine::debug_draw_buffers()
{
#if 0
	// ---------------------------------------------------------------------------
	// debug helper
	//
	// draw all the color attachments for the main frame buffer in quads at the
	// bottom of the viewport
	// ----------------------------------------------------------------------------

	g_engine->opengl_mgr.shaders["simple"].bind();

	auto num_color_attachments = frame_buffer.color_attachments.size();
	float_t scale_factor = 1.f / (float_t)num_color_attachments;
	float_t w = viewport_w * scale_factor;
	float_t h = viewport_h * scale_factor;
	Rect rc = { 0.f, viewport_h - h, w, h };

	std::vector<const char*> names = { "color", "glow", "pick" };

	{
		scoped_render_state;

		for (auto x = 0; x < num_color_attachments; ++x)
		{
			Render::draw_quad(frame_buffer.color_attachments[ x ].texture, rc );
			Render::draw_string( names[ x ], { rc.x, rc.y } );
			rc.x += w;
		}

		g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::opaque );
		g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::transparent );
	}

#endif
}

}
