
#include "app_header.h"

endless_breakout_game::endless_breakout_game()
{
}

void endless_breakout_game::init()
{
}

void endless_breakout_game::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_gameplay>() );
}

void endless_breakout_game::update()
{
	w_game::update();
}
