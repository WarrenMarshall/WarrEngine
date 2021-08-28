
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Game::Game( std::string internal_name )
{
	this->internal_name = internal_name;
}

void Game::init()
{
	if( flags.has_main_menu )
	{
		reset_scene_stack_to_main_menu();
	}

	new_game();
}

void Game::new_game()
{
	g_engine->new_physics_world();
	g_engine->scene_mgr.new_game();
}

void Game::update()
{
}

// removes all existing layers and sets up the game to be at the main menu

void Game::reset_scene_stack_to_main_menu()
{
}

}
