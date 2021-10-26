
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Life_Cycle::set( e_life_cycle life_cycle )
{
	_life_cycle = life_cycle;

	// adds a delay of a few frames to make sure that anyone
	// who needs to clean up re: this object, can do so.

	if( is_dying() and !death_delay )
	{
		death_delay = 20;
	}
}

e_life_cycle Life_Cycle::get()
{
	return _life_cycle;
}

bool Life_Cycle::is_alive() const
{
	return ( _life_cycle == e_life_cycle::alive );
}

bool Life_Cycle::is_dying() const
{
	return ( _life_cycle == e_life_cycle::dying );
}

bool Life_Cycle::is_dead() const
{
	return ( _life_cycle == e_life_cycle::dead );
}

void Life_Cycle::pre_update()
{
	if( death_delay > -1 )
	{
		death_delay--;
	}
}

}
