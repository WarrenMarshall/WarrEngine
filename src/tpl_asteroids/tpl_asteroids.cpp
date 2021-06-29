
#include "app_header.h"

using namespace war;

tpl_asteroids_game::tpl_asteroids_game( std::string name )
	: game( name )
{
}

void tpl_asteroids_game::init()
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

	g_engine->render_api.set_uniform( "ub_pixelate", true );
	g_engine->render_api.set_uniform( "u_pixelate_factor", 1.0f );

	g_engine->scenes.push<scene_starfield>();
	g_engine->scenes.push<scene_gameplay>();

	// generate vertices for each of the ship levels

	int sides = 3;

	for( int shape_idx = 0 ; shape_idx < max_ship_power_level ; ++shape_idx )
	{
		float angle = 0;
		float angle_step = 360.f / (float)( sides );

		for( auto x = 0 ; x < sides ; ++x )
		{
			ship_geo[ shape_idx ].verts.push_back( vec2::dir_from_angle( angle ) * my_game->ship_radius );
			angle += angle_step;
		}

		sides++;
	}
}

void tpl_asteroids_game::new_game()
{
	ship_power_level = 0;

	game::new_game();
}

void tpl_asteroids_game::update()
{
	game::update();
}

time_ms tpl_asteroids_game::get_ship_powerup_timer_duration()
{
	return 10'000 + ( my_game->ship_power_level * 5'000 );
}

// powers up the player ship to the next level

void tpl_asteroids_game::level_up_player_ship()
{
	ship_power_level++;
	ship_power_level = glm::min<int>( ship_power_level, max_ship_power_level - 1 );
}

bool tpl_asteroids_game::ship_is_at_max_level()
{
	return true;

	return ( ship_power_level == (max_ship_power_level - 1) );
}
