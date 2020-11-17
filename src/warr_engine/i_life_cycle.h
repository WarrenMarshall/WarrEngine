#pragma once

// ----------------------------------------------------------------------------

struct i_life_cycle
{
	e_life_cycle life_cycle = life_cycle::alive;

	virtual void set_life_cycle( e_life_cycle life_cycle );

	[[nodiscard]] bool is_alive();
	[[nodiscard]] bool is_dying();
	[[nodiscard]] bool is_dead();
};
