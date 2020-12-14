
#include "app_header.h"

hello_world_app::hello_world_app( const char* name )
	: w_game( name )
{
}

void hello_world_app::init()
{
	engine->layer_mgr->push<layer_default>();
}

void hello_world_app::new_game()
{
	w_game::new_game();
}

void hello_world_app::update()
{
	w_game::update();
}
