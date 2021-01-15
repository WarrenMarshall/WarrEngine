#pragma once

struct w_game
{
	w_game( std::string name );
	virtual ~w_game() = default;

	virtual void init();
	virtual void new_game();
	virtual void update();
	virtual void reset_layer_stack_to_main_menu();

	const char* get_game_name();

	std::string name;
};
