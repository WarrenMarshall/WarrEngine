
#include "app_header.h"

e_pong_ball::e_pong_ball()
{
	draw_debug_info = true;

	set_collision( clayer_ball, clayer_world | clayer_ball | clayer_paddle );

	add_component<ec_sprite>()->init( "pong_ball" );

	auto ecd = add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	auto f = ecd->add_fixture_circle( contact_id::ball, w_vec2::zero, 8 );
	f->SetRestitution( 0.5f );

	dir = w_vec2::get_random_unit();
	reset_velocity();
}

void e_pong_ball::reset_velocity()
{
	ec_b2d_body* ecb = get_component< ec_b2d_body>( component_type::b2d_body );

	dir.normalize();
	ecb->body->SetLinearVelocity( dir * speed );
}

// ----------------------------------------------------------------------------

e_pong_paddle::e_pong_paddle()
{
	draw_debug_info = true;

	set_collision( clayer_paddle, clayer_ball );

	auto ecd = add_component<ec_b2d_kinematic>();
	ecd->is_primary_body = true;
	auto f = ecd->add_fixture_box( contact_id::paddle, w_vec2::zero, 80, 12 );
}
