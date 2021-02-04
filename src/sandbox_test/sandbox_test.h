#pragma once

// ----------------------------------------------------------------------------

struct sandbox_test_app : w_game
{
	static std::string get_custom_name()
	{
		return "sandbox_test";
	}

	sandbox_test_app( std::string name );

	virtual void init() override;
	virtual void reset_layer_stack_to_main_menu() override;
	virtual void new_game() override;
	virtual void update() override;
};
