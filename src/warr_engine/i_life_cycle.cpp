
#include "master_pch.h"
#include "master_header.h"

void i_life_cycle::ilc_set( e_life_cycle life_cycle )
{
	this->life_cycle = life_cycle;

	// adds a delay of a few frames to make sure that anyone
	// who needs to clean up re: this object, can do so.
	if( life_cycle == life_cycle::dying )
	{
		ilc_update_count_death_delay = 2;
	}
}

bool i_life_cycle::ilc_is_alive()
{
	return ( life_cycle == life_cycle::alive );
}

bool i_life_cycle::ilc_is_dying()
{
	return ( life_cycle == life_cycle::dying );
}

bool i_life_cycle::ilc_is_dead()
{
	return ( life_cycle == life_cycle::dead );
}
