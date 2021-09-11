
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Life_Cycle_Mgr::set( e_life_cycle_t lc )
{
	_life_cycle = lc;

	// adds a delay of a few frames to make sure that anyone
	// who needs to clean up re: this object, can do so.
	if( is_dying() )
	{
		death_delay = 20;
	}
}

e_life_cycle_t Life_Cycle_Mgr::get()
{
	return _life_cycle;
}

bool Life_Cycle_Mgr::is_alive() const
{
	return ( _life_cycle == e_life_cycle::alive );
}

bool Life_Cycle_Mgr::is_dying() const
{
	return ( _life_cycle == e_life_cycle::dying );
}

bool Life_Cycle_Mgr::is_dead() const
{
	return ( _life_cycle == e_life_cycle::dead );
}

void Life_Cycle_Mgr::pre_update()
{
	// tick the death delay down, stopping at zero
	death_delay = glm::max( death_delay - 1, 0 );
}

}
