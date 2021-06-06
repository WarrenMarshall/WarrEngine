
using namespace war;

struct sandbox_test_app : game
{
	static std::string get_custom_name()
	{
		return "sandbox_test";
	}

	sandbox_test_app( std::string name );

	virtual void init() override;
	virtual void reset_scene_stack_to_main_menu() override;
};
