
#include "app_header.h"

hello_world_app::hello_world_app( std::string name )
	: w_game( name )
{
}

void hello_world_app::init()
{
	OPENGL->set_uniform( "ub_auto_glow", true );
	OPENGL->set_uniform( "u_auto_glow_lower_limit", 0.9f );

	//OPENGL->set_uniform( "ub_vignette", true );
	//OPENGL->set_uniform( "ub_crt_warp", true );
	//OPENGL->set_uniform( "ub_crt_scanlines", true );
	//OPENGL->set_uniform( "ub_crt_tint", false );

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
