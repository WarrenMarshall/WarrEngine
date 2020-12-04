
#include "app_header.h"

snake_pong::snake_pong( const char* name )
	: w_game( name )
{
}

void snake_pong::init()
{
}

void snake_pong::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push<layer_default>();

	for( int x = 0 ; x < 50 ; ++x )
	{
		log( "{}", engine->random->getf() );
	}
}

void snake_pong::update()
{
	w_game::update();
}
