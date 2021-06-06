#pragma once

// ----------------------------------------------------------------------------

struct i_life_cycle
{
	virtual void ilc_set( e_life_cycle life_cycle );

	[[nodiscard]] bool ilc_is_alive();
	[[nodiscard]] bool ilc_is_dying();
	[[nodiscard]] bool ilc_is_dead();

	// setting this to more than zero means that even if
	// this entity is dead in every way, it can't be deleted
	// until this many world updates have passed.
	//
	// this is kind of hacky, but it works. it gives things like
	// box2d a chance to stop talking about certain entities
	// for example.

	int ilc_update_count_death_delay = 0;

private:
	e_life_cycle life_cycle = life_cycle::alive;
};
