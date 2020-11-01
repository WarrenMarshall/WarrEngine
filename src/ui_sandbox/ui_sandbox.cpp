
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

	auto e = engine->layer_mgr->get_top()->add_entity<w_entity>();
	e->set_tag( "arrow_mesh" );
	e->set_position( { v_window_hw, v_window_hh + 64 } );
	e->set_scale( 3.0f );
	e->add_component<ec_mesh>()->init( "mesh_5sided" );
}

void ui_sandbox::update()
{
	w_game::update();
}
