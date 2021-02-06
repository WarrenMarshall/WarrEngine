
#include "master_pch.h"
#include "master_header.h"

w_game::w_game( std::string name )
{
	this->name = name;
}

void w_game::init()
{
	if( has_main_menu )
	{
		reset_layer_stack_to_main_menu();
	}

	new_game();
}

void w_game::new_game()
{
	engine->new_physics_world();
	engine->layer_mgr->new_game();
}

void w_game::update()
{
}

// removes all existing layers and sets up the game to be at the main menu

void w_game::reset_layer_stack_to_main_menu()
{
}

const char* w_game::get_game_name()
{
	return name.c_str();
}
