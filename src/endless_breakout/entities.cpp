
#include "app_header.h"

e_background_stars::e_background_stars()
{
	add_component<ec_emitter>()->init( "background_stars" );
	//add_component<ec_emitter>()->init( "background_fire_up" )->set_transform( { 0.0f, +v_window_hh }, 0, 1.0f );
	add_component<ec_emitter>()->init( "background_fire_down" )->set_transform( { 0.0f, -v_window_hh }, 0, 1.0f );
}

e_player::e_player()
{
	add_component<ec_sprite>()->init( "sub_tex_paddle_blue" )->set_transform( { 0.0f, 0.0f }, 0.0f, 0.5f );
}
