#pragma once

// ----------------------------------------------------------------------------

struct sandbox_test_app : w_game
{
	static std::string get_custom_name()
	{
		return "sandbox_test";
	}

	sandbox_test_app( std::string name );

	void init() override;
	void new_game() override;
	void update() override;
};
