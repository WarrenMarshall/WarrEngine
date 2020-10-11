
#include "app_header.h"

e_platformer_player::e_platformer_player()
{
	tag = "player";
	set_collision( clayer_player, clayer_world | clayer_coin | clayer_mover );
	//draw_debug_info = true;

	auto ec = add_component<ec_b2d_dynamic>();
	{
		ec->body->SetFixedRotation( true );
		ec->is_primary_body = true;

		auto f = ec->add_fixture_polygon(
			"player",
			w_vec2( 0.0f, 0.0f ),
			{
				{ -8, -8 },
				{ 8, -8 },
				{ 8, 4 },
				{ 4, 8 },
				{ -4, 8 },
				{ -8, 4 }
			}
		);

		ec->add_fixture_box( "s_on_ground", { 0.0f, 8.0f }, 12.0f, 4.0f )->SetSensor( true );
		ec->add_fixture_box( "s_can_drop_down", { 0.0f, 20.0f }, 12.0f, 16.0f )->SetSensor( true );
	}

	add_component<ec_sprite>()->init( "anim_player_idle" );
}

// ----------------------------------------------------------------------------

e_platformer_coin::e_platformer_coin()
{
	//draw_debug_info = true;

	auto ec = add_component<ec_b2d_dynamic>();
	{
		ec->body->SetFixedRotation( true );
		ec->is_primary_body = true;

		// world collisions
		set_collision( clayer_coin, clayer_world | clayer_coin | clayer_mover );
		auto f = ec->add_fixture_circle( "", w_vec2::zero, 8 );
		f->SetRestitution( 0.5f );

		// colliding with player
		set_collision( clayer_coin, clayer_player );
		ec->add_fixture_circle( "coin", w_vec2::zero, 4 )->SetSensor( true );
	}

	add_component<ec_sprite>()->init( "anim_coin" );
	//add_component<ec_emitter>()->init( "coin_idle_effect" );
}

// ----------------------------------------------------------------------------

e_platformer_mover::e_platformer_mover()
{
	draw_debug_info = true;

	auto ec = add_component<ec_b2d_kinematic>();
	{
		ec->body->SetFixedRotation( true );
		ec->is_primary_body = true;

		set_collision( clayer_mover, clayer_player | clayer_coin );
		auto f = ec->add_fixture_line( "world", w_vec2::zero, w_vec2( -32, 0 ), w_vec2( 32, 0 ) );
	}
}
