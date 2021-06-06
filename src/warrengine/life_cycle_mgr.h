
namespace war
{

struct life_cycle_mgr
{
	void set( e_life_cycle life_cycle );
	e_life_cycle get();

	[[nodiscard]] bool is_alive() const;
	[[nodiscard]] bool is_dying() const;
	[[nodiscard]] bool is_dead() const;

	void pre_update();

	// setting this to more than zero means that even if
	// this entity is dead in every way, it can't be deleted
	// until this many world updates have passed.
	//
	// this is kind of hacky, but it works. it gives things like
	// box2d a chance to stop talking about certain entities
	// for example.

	int death_delay = 0;

private:
	e_life_cycle _life_cycle = life_cycle::alive;
};

}
