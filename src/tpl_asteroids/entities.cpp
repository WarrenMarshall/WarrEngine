
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

e_player::e_player()
{
/*
	{
		auto ec = add_component<ec_primitive_shape>();
		ec->add_shape( primitive_shape::filled_circle, 8.0f );
	}
*/

	gun_timer = timer( 1500 );
}

void e_player::fire()
{
	for( auto& ship : satellite_ships )
	{
		ship->fire();
	}
}

void e_player::update()
{
	entity::update();

	// when it's time to fire the weapon, fire it and then reset the timer to
	// match whatever the players current ship power level is.
	//
	// a more powerful ship = faster firing rate

	if( gun_timer.get_elapsed() )
	{
		fire();
		gun_timer = timer( 1500 - (50 * my_game->ship_power_level ) );
	}
}

// ----------------------------------------------------------------------------

e_player_ship::e_player_ship()
{
	set_tag( H( "player" ) );
	{
		{
			add_component<player_ship_component>();
		}
	}
}

void e_player_ship::fire()
{
	if( is_outside_playfield() )
	{
		return;
	}

	int sides = my_game->ship_power_level + 3;
	float angle = 0;
	float angle_step = 360.f / (float)( sides );

	for( auto x = 0 ; x < sides ; ++x )
	{
		auto e = g_engine->scenes.get_top()->add_entity<e_player_bullet>();

		auto dir = vec2::dir_from_angle( get_angle() + angle );

		e->set_pos( get_pos() + ( dir * my_game->ship_radius ) );
		e->set_angle( get_angle() + angle );

		angle += angle_step;
	}
}

bool e_player_ship::is_outside_playfield()
{
	// if the ship is too far outside the playfield, skip any attempt to spawn bullets
	rect rc_playfield { -viewport_hw, -viewport_hh, viewport_w, viewport_h };
	rc_playfield.grow( 32.f );

	return !rc_playfield.contains_point( get_pos() );
}

void e_player_ship::update()
{
	entity::update();

	float rotation_speed = my_game->ship_base_rotation_speed + ( my_game->ship_per_level_rotation_speed * my_game->ship_power_level );

	add_delta_angle( fixed_time_step::per_second( rotation_speed ) );
}

// ----------------------------------------------------------------------------

e_player_bullet::e_player_bullet()
{
	{
		add_component<player_bullet_component>();
	}
}

void e_player_bullet::update()
{
	entity::update();

	float movement_speed = my_game->bullet_base_movement_speed;

	if( get_life_cycle()->is_alive() )
	{
		// move bullet forward along facing vector
		vec2 dir = vec2::dir_from_angle( get_angle() );
		add_delta_pos( fixed_time_step::per_second( dir * movement_speed ) );


		rect rc_playfield { -viewport_hw, -viewport_hh, viewport_w, viewport_h };
		rc_playfield.grow( 8.f );

		// if bullet is outside the play area, delete it
		if( !rc_playfield.contains_point( get_pos() ) )
		{
			set_life_cycle( life_cycle::dying );
		}
	}
}
