
#include "master_pch.h"
#include "master_header.h"

w_game::w_game( const char* name )
{
	this->name = name;
}

void w_game::init()
{
}

void w_game::new_game()
{
	engine->new_physics_world();
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
	return name;
}
