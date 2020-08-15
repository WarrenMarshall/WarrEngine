#pragma once

// ----------------------------------------------------------------------------

struct GE_API i_lifecycle
{
	e_lifecycle lifecycle = lifecycle::alive;

	virtual void set_life_cycle( e_lifecycle lifecycle );
	bool is_alive();
	bool is_dying();
	bool is_dead();
};
