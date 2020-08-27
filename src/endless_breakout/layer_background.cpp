
#include "app_header.h"

layer_background::layer_background()
{
	draws_completely_solid = true;
}

void layer_background::push()
{
	background_gradient = engine->get_asset<a_gradient>( "background_gradient" );

	game->spawn_entity<e_background_stars>( { v_window_hw, v_window_hh } );
}

void layer_background::draw()
{
	game->draw_entities();
}
