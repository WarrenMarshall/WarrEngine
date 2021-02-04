
#include "app_header.h"

sandbox_test_app::sandbox_test_app( std::string name )
	: w_game( name )
{
}

void sandbox_test_app::init()
{
	//OPENGL->set_uniform( "ub_auto_glow", true );
	//OPENGL->set_uniform( "u_auto_glow_lower_limit", 0.9f );

	//OPENGL->set_uniform( "ub_vignette", true );
	//OPENGL->set_uniform( "ub_crt_warp", true );
	//OPENGL->set_uniform( "ub_crt_scanlines", true );
	//OPENGL->set_uniform( "ub_crt_tint", false );

	engine->layer_mgr->push<layer_main_menu>();
}

void sandbox_test_app::reset_layer_stack_to_main_menu()
{
	engine->layer_mgr->clear_stack();

	engine->layer_mgr->push<layer_main_menu>();
}

void sandbox_test_app::new_game()
{
	w_game::new_game();
}

void sandbox_test_app::update()
{
	w_game::update();
}
