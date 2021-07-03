
using namespace war;

#define my_game ((gravity_flip_game*)g_base_game.get())

// ----------------------------------------------------------------------------

struct gravity_flip_game : game
{
	gravity_flip_game();

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
};
