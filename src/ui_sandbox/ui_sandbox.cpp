
#include "app_header.h"

ui_sandbox::ui_sandbox( const char* name )
	: w_game( name )
{
}

void ui_sandbox::init()
{
	engine->layer_mgr->push<layer_default>();
}

void ui_sandbox::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push<layer_default>();
}

void ui_sandbox::update()
{
	w_game::update();
}
