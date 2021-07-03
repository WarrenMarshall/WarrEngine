
#include "app_header.h"

using namespace war;

sandbox_test_app::sandbox_test_app()
	: game( "sandbox_test" )
{
	flags.has_main_menu = true;
}

void sandbox_test_app::init()
{
#if 0
	//g_engine->render_api.set_uniform( "ub_vignette", true );

	//g_engine->render_api.set_uniform( "ub_chromatic_aberration", true );
	//g_engine->render_api.set_uniform( "ub_film_grain", true );
	//g_engine->render_api.set_uniform( "ub_crt_warp", true );
	//g_engine->render_api.set_uniform( "ub_crt_scanlines", true );
	g_engine->render_api.set_uniform( "ub_crt_tint", true );

	//g_engine->render_api.set_uniform( "ub_desaturation", true );
	//g_engine->render_api.set_uniform( "ub_greyscale", true );
	//g_engine->render_api.set_uniform( "ub_sepia", true );
	//g_engine->render_api.set_uniform( "ub_invert", true );
	//g_engine->render_api.set_uniform( "u_colorize", color( 1.f, 1.f, 0.f, 0.05f ) );
	//g_engine->render_api.set_uniform( "u_color_overlay", color( 0.f, 1.f, 0.f, 0.1f ) );

	g_engine->render_api.set_uniform( "ub_pixelate", true );
	g_engine->render_api.set_uniform( "u_pixelate_factor", 1.f );
#endif

	g_engine->scenes.push<scene_main_menu>();
}

void sandbox_test_app::reset_scene_stack_to_main_menu()
{
	g_engine->scenes.clear_stack();

	g_engine->scenes.push<scene_main_menu>();
}
