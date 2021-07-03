
using namespace war;

struct hello_world_app : game
{
	hello_world_app();

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
};
