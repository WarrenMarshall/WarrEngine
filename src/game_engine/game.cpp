
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
	// Create a new world for Box2D at the start of each game. This just seems
	// to help keep everything stable and removes confusion.
	engine->box2d_world = std::make_unique<b2World>( b2Vec2( 0.0f, b2d_gravity_default ) );
}

void w_game::update()
{
}

// removes all existing layers and sets up the game to be at the main menu

void w_game::return_to_main_menu()
{
}
