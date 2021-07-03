
#include "app_header.h"

using namespace war;

gravity_flip_game::gravity_flip_game()
	: game( "gravity_flip" )
{
}

void gravity_flip_game::init()
{
#if 0
	g_engine->render_api.set_uniform( "ub_vignette", true );
	g_engine->render_api.set_uniform( "ub_crt_warp", true );
	g_engine->render_api.set_uniform( "ub_crt_tint", true );
	g_engine->render_api.set_uniform( "ub_chromatic_aberration", true );
	g_engine->render_api.set_uniform( "ub_greyscale", true );
	g_engine->render_api.set_uniform( "u_color_overlay", make_color( color::green, 0.1f ) );
	g_engine->render_api.set_uniform( "ub_film_grain", true );
#endif

	g_engine->scenes.push<scene_starfield>();
	g_engine->scenes.push<scene_gameplay>();
}

void gravity_flip_game::new_game()
{
	game::new_game();
}

void gravity_flip_game::update()
{
	game::update();
}
