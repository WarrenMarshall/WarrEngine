
#include "app_header.h"

entity_sandbox_app::entity_sandbox_app( std::string_view name )
	: w_game( name )
{
}

void entity_sandbox_app::init()
{
}

void entity_sandbox_app::new_game()
{
	w_game::new_game();

	engine->layer_mgr->push<layer_default>();
}

void entity_sandbox_app::update()
{
	w_game::update();
}
