
#include "master_pch.h"
#include "master_header.h"

g_custom_game::g_custom_game()
{
	font = engine->get_asset<a_font>( "medieval_font" );
}

void g_custom_game::new_game()
{
	w_game::new_game();

	// todo : it feels like this could be better encapsulated. 3 steps to spawn something is a little bleh.
	player = game->add_entity<e_player>();
	player->set_transform( w_vec3( 0, 0, 0 ), 0, 1.0f );
	player->post_spawn();
}

void g_custom_game::update()
{
	w_game::update();

}

void g_custom_game::update_fts()
{
	w_game::update_fts();
}
