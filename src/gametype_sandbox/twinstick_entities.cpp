
#include "app_header.h"

e_twinstick_player::e_twinstick_player()
{
	draw_debug_info = true;

	set_collision( clayer_player, clayer_world );

	auto ecd = add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	auto f = ecd->add_fixture_circle( contact_id::player, w_vec2::zero, 12 );
	f->SetFriction( 1.0f );
	f->SetDensity( 10.f );

	add_component<ec_sprite>()->init( "twinstick_player" );
}
