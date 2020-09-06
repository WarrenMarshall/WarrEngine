
#include "app_header.h"

main_app::main_app()
{
}

void main_app::init()
{
}

void main_app::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push( std::make_unique<layer_default>() );
}

void main_app::update()
{
	w_game::update();
}
