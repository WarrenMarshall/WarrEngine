
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void life_cycle_mgr::set( e_life_cycle lc )
{
	_life_cycle = lc;

	// adds a delay of a few frames to make sure that anyone
	// who needs to clean up re: this object, can do so.
	if( is_dying() )
	{
		death_delay = 20;
	}
}

e_life_cycle life_cycle_mgr::get()
{
	return _life_cycle;
}

bool life_cycle_mgr::is_alive() const
{
	return ( _life_cycle == life_cycle::alive );
}

bool life_cycle_mgr::is_dying() const
{
	return ( _life_cycle == life_cycle::dying );
}

bool life_cycle_mgr::is_dead() const
{
	return ( _life_cycle == life_cycle::dead );
}

void life_cycle_mgr::pre_update()
{
	// tick the death delay down, stopping at zero
	death_delay = glm::max( death_delay - 1, 0 );
}

}
