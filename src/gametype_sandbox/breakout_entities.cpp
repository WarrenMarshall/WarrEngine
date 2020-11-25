
#include "app_header.h"

e_breakout_ball::e_breakout_ball()
{
	set_collision( clayer_ball, clayer_world | clayer_ball | clayer_paddle );

	add_component<ec_sprite>()->init( "pong_ball" );

	auto ecd = add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	ecd->body->SetFixedRotation( true );
	auto f = ecd->add_fixture_circle( "ball", w_vec2::zero, 8 );

	dir = w_vec2::get_random_unit();
	reset_velocity();
}

void e_breakout_ball::reset_velocity()
{
	ec_b2d_body* ecb = get_component< ec_b2d_body>( component_type::b2d_body );

	dir.normalize();
	ecb->body->SetLinearVelocity( ( dir * speed ).as_b2Vec2() );
}

void e_breakout_ball::update()
{
	ec_b2d_body* ecb = get_component< ec_b2d_body>( component_type::b2d_body );
	w_vec2 vel = w_vec2( ecb->body->GetLinearVelocity() );
	vel.normalize();

	ecb->body->SetLinearVelocity( ( vel * speed ).as_b2Vec2() );
}

// ----------------------------------------------------------------------------

e_breakout_paddle::e_breakout_paddle()
{
	set_collision( clayer_paddle, clayer_ball );

	auto ecd = add_component<ec_b2d_kinematic>();
	ecd->is_primary_body = true;
	auto f = ecd->add_fixture_box( "paddle", w_vec2::zero, 80, 12 );
}
