
#include "app_header.h"

using namespace war;
Gauntlet_App::Gauntlet_App()
	: Game( "gauntlet" )
{
}

void Gauntlet_App::init()
{
#if 1
	//g_engine->render_api.set_uniform( "ub_vignette", true );
	//g_engine->render_api.set_uniform( "ub_chromatic_aberration", true );
	//g_engine->render_api.set_uniform( "ub_film_grain", true );
	//g_engine->render_api.set_uniform( "ub_crt_warp", true );
	//g_engine->render_api.set_uniform( "ub_crt_scanlines", true );
	//g_engine->render_api.set_uniform( "ub_crt_tint", true );

	//g_engine->render_api.set_uniform( "ub_desaturation", true );
	//g_engine->render_api.set_uniform( "ub_greyscale", true );
	//g_engine->render_api.set_uniform( "ub_sepia", true );
	//g_engine->render_api.set_uniform( "ub_invert", true );
	//g_engine->render_api.set_uniform( "u_colorize", color( 1.f, 1.f, 0.f, 0.05f ) );
	//g_engine->render_api.set_uniform( "u_color_overlay", color( 0.f, 1.f, 0.f, 0.1f ) );


	//g_engine->render_api.set_uniform( "u_pixelate_factor", 0.0f );
#endif

	g_engine->scene_mgr.push<Scene_Default>();
}

void Gauntlet_App::new_game()
{
	Game::new_game();
}

void Gauntlet_App::update()
{
	Game::update();
}
