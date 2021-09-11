
using namespace war;

struct Sandbox_Test_App : Game
{
	Sandbox_Test_App();

	virtual void init() override;
	virtual void reset_scene_stack_to_main_menu() override;
};
