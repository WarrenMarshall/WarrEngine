
#include "master_pch.h"
#include "master_header.h"

g_custom_game::g_custom_game()
{
	font = engine->get_asset<a_font>( "medieval_font" );
}

void g_custom_game::new_game()
{
	w_game::new_game();

	player = game->spawn_entity<e_player>( w_vec3::zero, 30.f );
}

void g_custom_game::update()
{
	w_game::update();

}

void g_custom_game::update_fts()
{
	w_game::update_fts();
}
