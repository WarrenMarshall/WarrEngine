
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

/*
e_player::e_player()
{
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
*/
