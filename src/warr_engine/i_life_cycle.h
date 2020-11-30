#pragma once

// ----------------------------------------------------------------------------

struct i_life_cycle
{
	virtual void ilc_set( e_life_cycle life_cycle );

	[[nodiscard]] bool ilc_is_alive();
	[[nodiscard]] bool ilc_is_dying();
	[[nodiscard]] bool ilc_is_dead();

private:
	e_life_cycle life_cycle = life_cycle::alive;

};
