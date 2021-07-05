
using namespace war;

#define my_game ((gravity_flip_game*)g_base_game.get())

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned coll_player = collision_bits.get();
static const unsigned coll_world = collision_bits.next();

// ----------------------------------------------------------------------------

struct gravity_flip_game : game
{
	gravity_flip_game();

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
};
