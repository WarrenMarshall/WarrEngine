
#include "master_pch.h"
#include "master_header.h"

g_asteroids::g_asteroids()
{
	font = engine->get_asset<a_font>( "asteroids_font" );
}

void g_asteroids::new_game()
{
	w_game::new_game();

	// player - stands in the middle of the playing field
	player = game->add_entity<e_player>();
	player->set_transform( w_vec3( 0, 0, 0 ), 0, 1.0f );
	player->post_spawn();
}

void g_asteroids::update()
{
	w_game::update();

}

void g_asteroids::update_fts()
{
	w_game::update_fts();
}
