
#include "app_header.h"

input_sandbox_app::input_sandbox_app( const char* name )
	: w_game( name )
{
}

void input_sandbox_app::init()
{
}

void input_sandbox_app::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push<layer_default>();
}

void input_sandbox_app::update()
{
	w_game::update();
}
