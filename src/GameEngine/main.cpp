
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------
// globals

// log file
std::unique_ptr<w_logfile> logfile;

// engine
std::unique_ptr<w_engine> engine;

// game
std::unique_ptr<g_custom_game> game;

// command line flags
bool g_allow_hot_reload;

// ----------------------------------------------------------------------------

int main( int argc, char* argv[] )
{
	try
	{
#if defined(FINALRELEASE)
		ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

		stbi_set_flip_vertically_on_load( 1 );

		// get the log file running so we can immediately start writing into it
		logfile = std::make_unique<w_logfile>();
		logfile->init( "game_engine" );

		// create the game engine
		engine = std::make_unique<w_engine>();

		{	// starting
			logfile->time_stamp( "Started" );
		}

		{	// engine
			log_msg( "Initializing engine" );
			engine->init();
		}

		{	// command line parsing

			g_allow_hot_reload = false;

			for( int a = 1; a < argc; ++a )
			{
				std::string arg( argv[a] );

				// +hot_reload - enable hot reloading of asset definition files (defaults to false)
				if( arg == "+hot_reload" )
				{
					log_msg( "command line : +hot_reload : hot reloading enabled" );
					g_allow_hot_reload = true;
				}
			}
		}

		{	// window
			log_msg( "Creating window" );
			if( !engine->window->init( "Game Engine [Endless Adventure]" ) )
				return -1;
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
			if( !BASS_Init( -1, 44100, 0, 0, NULL ) )
				log_warning( "BASS Audio : init failed!" );
		}

		{
			// read asset definitions and cache them
			log_msg( "Caching asset definition files (*.asset_def)..." );
			engine->cache_asset_definition_files();

			// look through the cached asset definitions and load any assets required
			// into the asset cache
			log_msg( "Precaching resources from definition files..." );
			engine->precache_asset_resources();
		}

		{
			log_msg( "Initializing game" );
			game = std::make_unique<g_custom_game>();
			game->new_game();
		}

		// input initialization

		log_msg( "Initializing input" );
		engine->input_mgr->init();

		// initial layer set up

		engine->layer_mgr->push( std::make_unique<layer_background>() );
		engine->layer_mgr->push( std::make_unique<layer_worldviewport>() );
		engine->layer_mgr->push( std::make_unique<layer_editor>() );

		engine->is_running = true;
		engine->time->init();

		// initialize random internals
		{
			// used for wireframe drawing
			engine->white_wire = engine->get_asset<a_subtexture>( "engine_white_wire" );
			engine->white_wire->tex->render_buffer->prim_type = GL_LINES;

			// used for solid drawing
			engine->white_solid = engine->get_asset<a_subtexture>( "engine_white_solid" );

			// #configfile - needs to be replaced with some sort of config file setting
			engine->ui_mgr->init( "ui_cursor" );

			// using a custom mouse cursor, so hide the system mouse
			engine->window->set_mouse_mode( e_mouse_mode::hidden );
		}

		/*
			main game loop
		*/

		RENDER->init_projection();

		while( engine->is_running )
		{
			/*
				event processing
			*/
			// #todo - this magic value is arbitrary. is this ok or do we need to do something fancier?
			glfwWaitEventsTimeout( 0.001 );

			/*
				update core engine stuff - time, timers, etc
			*/

			engine->time->update();
			engine->hot_reload_timer->update();

			/*
				process user input
			*/

			engine->input_mgr->update();

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
				at regular intervals, check if any assets need to be reloaded
			*/

			if( engine->hot_reload_timer->get_elapsed_count() )
			{
				engine->update_hot_reload();
			}

			/*
				draw everything
			*/

			// whatever remaining ms are left in engine->time->fts_accum_ms should be passed
			// to the render functions for interpolation/prediction
			//
			// it is passed a percentage for easier use : 0.0f-1.0f

			RENDER->begin_frame( engine->time->fts_accum_ms / w_time::FTS_step_value_ms );
			{
				engine->layer_mgr->draw();
				game->draw_entities();
				engine->ui_mgr->draw_top_level();
				engine->draw();
			}
			RENDER->end_frame();
		}

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
		engine->input_mgr->deinit();

		log_msg( "Shutting down engine" );
		engine->deinit();
	}
	catch( std::exception& e )
	{
		log_msg( "!! EXCEPTION" );
		log_msg( "\t%s", e.what() );

		MessageBoxA( nullptr, e.what(), "Exception!", MB_OK );
	}

	// Do this last so we can log right up until the last moment
	logfile->time_stamp( "Ended" );
	log_msg( "Finished!" );
	logfile->deinit();

	return 0;
}
