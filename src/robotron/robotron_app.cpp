
#include "app_header.h"

using namespace war;
Robotron_App::Robotron_App()
	: Game( "robotron" )
{
}

void Robotron_App::init()
{
#if 1
	g_engine->opengl_mgr.set_uniform_bool( "ub_crt_tint", true );
	g_engine->opengl_mgr.set_uniform_float( "u_crt_tint_scaling_factor", 1.0f );
	//g_engine->render_api.set_uniform( "ub_vignette", true );
	//g_engine->render_api.set_uniform( "ub_chromatic_aberration", true );
	//g_engine->render_api.set_uniform( "ub_film_grain", true );
	//g_engine->render_api.set_uniform( "ub_crt_warp", true );
	//g_engine->render_api.set_uniform( "ub_crt_scanlines", true );
	//g_engine->render_api.set_uniform( "ub_crt_tint", true );

	//g_engine->render_api.set_uniform( "ub_desaturation", true );
	//g_engine->render_api.set_uniform( "u_colorize", color( 1.f, 1.f, 0.f, 0.05f ) );
	//g_engine->render_api.set_uniform( "u_color_overlay", color( 0.f, 1.f, 0.f, 0.1f ) );


	//g_engine->render_api.set_uniform( "u_pixelate_factor", 0.0f );
#endif

	flags.has_main_menu = true;

	g_engine->scene_mgr.push<Scene_Background>();
	g_engine->scene_mgr.push<Scene_Menu>();
	g_engine->scene_mgr.push<Scene_Gameplay>();
}

void Robotron_App::new_game()
{
	Game::new_game();
}

void Robotron_App::update()
{
	Game::update();
}

void Robotron_App::reset_scene_stack_to_main_menu()
{
	g_engine->scene_mgr.push<Scene_Background>();
	g_engine->scene_mgr.push<Scene_Menu>();
}
