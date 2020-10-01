
#include "master_pch.h"
#include "master_header.h"

w_game::w_game( std::string_view name )
{
	this->name = std::string( name );
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

void w_game::return_to_main_menu()
{
}
