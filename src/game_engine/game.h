#pragma once

struct w_game
{
	w_game( std::string_view name );
	virtual ~w_game() = default;

	virtual void init();
	virtual void new_game();
	virtual void update();
	virtual void reset_layer_stack_to_main_menu();
	virtual std::string get_game_name();

	std::string name = "";
};
