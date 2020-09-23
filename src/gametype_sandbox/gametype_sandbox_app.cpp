
#include "app_header.h"

gametype_sandbox_app::gametype_sandbox_app( std::string_view name )
	: w_game( name )
{
}

void gametype_sandbox_app::init()
{
	w_game::init();
	plat_physics = std::make_unique<platformer_physics>();
}

void gametype_sandbox_app::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push<layer_main_menu>();
}

void gametype_sandbox_app::update()
{
	w_game::update();
}
