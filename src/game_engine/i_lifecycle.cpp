
#include "master_pch.h"
#include "master_header.h"

void i_lifecycle::set_life_cycle( e_lifecycle lifecycle )
{
	this->lifecycle = lifecycle;
}

bool i_lifecycle::is_alive()
{
	return ( lifecycle == lifecycle::alive );
}

bool i_lifecycle::is_dying()
{
	return ( lifecycle == lifecycle::dying );
}

bool i_lifecycle::is_dead()
{
	return ( lifecycle == lifecycle::dead );
}