
#include "master_pch.h"
#include "master_header.h"

void i_lifecycle::set_life_cycle( e_life_cycle life_cycle )
{
	this->life_cycle = life_cycle;
}

bool i_lifecycle::is_alive()
{
	return ( life_cycle == life_cycle::alive );
}

bool i_lifecycle::is_dying()
{
	return ( life_cycle == life_cycle::dying );
}

bool i_lifecycle::is_dead()
{
	return ( life_cycle == life_cycle::dead );
}
