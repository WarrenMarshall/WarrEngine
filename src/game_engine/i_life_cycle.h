#pragma once

// ----------------------------------------------------------------------------

struct i_life_cycle
{
	e_life_cycle life_cycle = life_cycle::alive;

	virtual void set_life_cycle( e_life_cycle life_cycle );

	_NODISCARD bool is_alive();
	_NODISCARD bool is_dying();
	_NODISCARD bool is_dead();
};
