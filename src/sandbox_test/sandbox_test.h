
using namespace war;

struct sandbox_test_app final : Game
{
	sandbox_test_app();

	virtual void init() override;
	virtual void reset_scene_stack_to_main_menu() override;
};
