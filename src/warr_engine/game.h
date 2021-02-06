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

	// set to TRUE if this app has a main menu to return to. setting to FALSE
	// will remove the "main menu" button from the engine ESC menu.
	bool has_main_menu = false;
};
