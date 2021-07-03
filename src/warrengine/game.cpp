
#include "master_pch.h"
#include "master_header.h"

namespace war
{

game::game( std::string internal_name )
{
	this->internal_name = internal_name;
}

void game::init()
{
	if( flags.has_main_menu )
	{
		reset_scene_stack_to_main_menu();
	}

	new_game();
}

void game::new_game()
{
	g_engine->new_physics_world();
	g_engine->scenes.new_game();
}

void game::update()
{
}

// removes all existing layers and sets up the game to be at the main menu

void game::reset_scene_stack_to_main_menu()
{
}

}
