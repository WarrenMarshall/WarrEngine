#pragma once

// ----------------------------------------------------------------------------

struct hello_world_app : w_game
{
	static std::string get_custom_name()
	{
		return "hello_world";
	}

	hello_world_app( std::string name );

	void init() override;
	void new_game() override;
	void update() override;
};
