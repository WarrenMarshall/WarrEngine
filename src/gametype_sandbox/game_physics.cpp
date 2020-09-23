
#include "app_header.h"

// ----------------------------------------------------------------------------

const float platformer_physics::player_move_force_s = 7.5f;
const float platformer_physics::player_move_force_max = 1.25f;
const float platformer_physics::player_jump_force = 3.0f;

platformer_physics::platformer_physics()
	: w_contact_listener()
{
	timer_jump_limiter = std::make_unique<w_timer>( 100 );
}

void platformer_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	timer_jump_limiter->update();

	if( sensor_ids_match( { sensor_id::ground, sensor_id::world } ) )
	{
		player_on_ground++;
	}

	if( sensor_ids_match( { sensor_id::area, sensor_id::world } ) )
	{
		player_drop_down_blocked++;
	}
}

void platformer_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( sensor_ids_match( { sensor_id::ground, sensor_id::world } ) )
	{
		player_on_ground--;
	}

	if( sensor_ids_match( { sensor_id::area, sensor_id::world } ) )
	{
		player_drop_down_blocked--;
	}
}

bool platformer_physics::can_jump()
{
	return (player_on_ground > 0);
}

bool platformer_physics::in_air()
{
	return ( player_on_ground == 0 );
}

bool platformer_physics::can_drop_down()
{
	return ( player_drop_down_blocked == 0 );
}
