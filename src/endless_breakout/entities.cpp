
#include "app_header.h"

// ----------------------------------------------------------------------------

e_player::e_player()
{
	add_component<ec_sprite>()->init( "sub_tex_paddle_blue" )->set_transform( { 0.0f, 0.0f }, 0.0f, 1.0f );
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	add_component<ec_sprite>()->init( "sub_tex_ball_blue" )->set_transform( { 0.0f, 0.0f }, 0.0f, 1.0f );
}

void e_ball::update()
{
	w_entity::update();

	pos.add( cache.forces );

	w_force* force = forces.back().get();
	if( pos.x > v_window_w )	force->dir.x *= -1.0f;
	if( pos.x < 0.0f )			force->dir.x *= -1.0f;
	if( pos.y > v_window_h )	force->dir.y *= -1.0f;
	if( pos.y < 0.0f )			force->dir.y *= -1.0f;
}
