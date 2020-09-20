#pragma once

// ----------------------------------------------------------------------------

struct i_life_cycle
{
	e_life_cycle life_cycle = life_cycle::alive;

	virtual void set_life_cycle( e_life_cycle life_cycle );

	bool is_alive();
	bool is_dying();
	bool is_dead();
};
