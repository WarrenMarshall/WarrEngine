
#include "app_header.h"

// ----------------------------------------------------------------------------

w_pong_physics::w_pong_physics()
	: w_contact_listener()
{
}

void w_pong_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	if( sensor_ids_match( { sensor_id::ball, sensor_id::world } ) )
	{
		log_msg( "WALL HIT!" );
	}
}

void w_pong_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( sensor_ids_match( { sensor_id::ball, sensor_id::world } ) )
	{
	}
}
