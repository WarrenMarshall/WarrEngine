
#include "app_header.h"

e_platformer_level::e_platformer_level()
{
	set_collision( clayer_world, clayer_player | clayer_coin );

	add_component<ec_b2d_static>();
	add_component<ec_tilemap>();
}

// ----------------------------------------------------------------------------

e_platformer_player::e_platformer_player()
{
	tag = "player";
	set_collision( clayer_player, clayer_world | clayer_coin );

	auto ec = add_component<ec_b2d_dynamic>();
	{
		const float radius = 6.5f;

		ec->body->SetFixedRotation( true );
		ec->is_primary_body = true;

		auto f = ec->add_fixture_circle(
			"player",
			w_vec2(0.0f,0.0f), radius );

		ec->add_fixture_box( "s_on_ground", { 0.0f, 8.0f }, 6.0f, 4.0f )->SetSensor( true );
		ec->add_fixture_circle( "s_can_drop_down", { 0.0f, radius * 3.0f }, radius )->SetSensor( true );
	}

	add_component<ec_sprite>()->init( "anim_player_idle" );
}

// ----------------------------------------------------------------------------

e_platformer_coin::e_platformer_coin()
{
	auto ec = add_component<ec_b2d_dynamic>();
	{
		ec->body->SetFixedRotation( true );
		ec->is_primary_body = true;

		// world collisions
		set_collision( clayer_coin, clayer_world | clayer_coin );
		auto f = ec->add_fixture_circle( "", w_vec2::zero, 8 );
		f->SetRestitution( 0.5f );

		// colliding with player
		set_collision( clayer_coin, clayer_player );
		ec->add_fixture_circle( "coin", w_vec2::zero, 4 )->SetSensor( true );
	}

	add_component<ec_sprite>()->init( "anim_coin" );
}
