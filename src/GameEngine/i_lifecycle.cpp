
#include "master_pch.h"
#include "master_header.h"

void i_lifecycle::set_life_cycle( e_lifecycle lifecycle )
{
	this->lifecycle = lifecycle;
}

bool i_lifecycle::is_alive()
{
	return ( lifecycle == e_lifecycle::alive );
}

bool i_lifecycle::is_dying()
{
	return ( lifecycle == e_lifecycle::dying );
}

bool i_lifecycle::is_dead()
{
	return ( lifecycle == e_lifecycle::dead );
}
