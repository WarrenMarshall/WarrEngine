
#include "master_pch.h"
#include "master_header.h"

void i_life_cycle::set_life_cycle( e_life_cycle life_cycle )
{
	this->life_cycle = life_cycle;
}

bool i_life_cycle::is_alive()
{
	return ( life_cycle == life_cycle::alive );
}

bool i_life_cycle::is_dying()
{
	return ( life_cycle == life_cycle::dying );
}

bool i_life_cycle::is_dead()
{
	return ( life_cycle == life_cycle::dead );
}
