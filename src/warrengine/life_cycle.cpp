
#include "master_pch.h"

namespace war
{

void Life_Cycle::set( e_life_cycle life_cycle )
{
	flags = ( i32 )( life_cycle );

	// adds a delay of a few frames to make sure that anyone
	// who needs to clean up re: this object, can do so.

	if( is_dying() and !death_delay )
	{
		death_delay = 20;
	}
}

i32 Life_Cycle::get()
{
	return ( i32 )( flags );
}

bool Life_Cycle::is_alive() const
{
	return ( ( i32 )( flags ) & ( i32 )( e_life_cycle::alive ) );
}

bool Life_Cycle::is_dying() const
{
	return ( ( i32 )( flags ) & ( i32 )( e_life_cycle::dying ) );
}

bool Life_Cycle::is_dead() const
{
	return ( ( i32 )( flags ) & ( i32 )( e_life_cycle::dead ) );
}

void Life_Cycle::pre_update()
{
	if( death_delay > -1 )
	{
		death_delay--;
	}
}

}
