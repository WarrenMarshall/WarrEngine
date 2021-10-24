
using namespace war;

struct Robotron_App final : Game
{
	Robotron_App();

	virtual void init() override;
	virtual void new_game() override;
	virtual void update() override;
	virtual void reset_scene_stack_to_main_menu() override;
};
