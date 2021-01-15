#pragma once

// ----------------------------------------------------------------------------

struct snake_pong : w_game
{
	static std::string get_custom_name()
	{
		return "snake_pong";
	}

	snake_pong( std::string name );

	virtual void init() override;
	virtual void reset_layer_stack_to_main_menu() override;
	virtual void new_game() override;
};
