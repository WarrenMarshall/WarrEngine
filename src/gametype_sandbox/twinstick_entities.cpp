
#include "app_header.h"

e_twinstick_player::e_twinstick_player()
{
	tag = "player";

	draw_debug_info = true;

	set_collision( clayer_player, clayer_world );

	auto ecd = add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	auto f = ecd->add_fixture_circle( "player", w_vec2::zero, 10 );

	ecd->body->SetFixedRotation( true );

	f->SetDensity( 1.0f );
	ecd->body->SetLinearDamping( 10.0f );

	add_component<ec_sprite>()->init( "twinstick_player" );
}
