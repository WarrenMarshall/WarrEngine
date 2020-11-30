
#include "master_pch.h"
#include "master_header.h"

void i_life_cycle::ilc_set( e_life_cycle life_cycle )
{
	this->life_cycle = life_cycle;
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
