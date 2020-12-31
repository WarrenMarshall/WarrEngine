
#include "app_header.h"

snake_pong::snake_pong( const char* name )
	: w_game( name )
{
}

void snake_pong::init()
{
	w_game::init();

	OPENGL->set_uniform( "ub_vignette", true );
	OPENGL->set_uniform( "ub_crt_warp", true );
	//OPENGL->set_uniform( "ub_crt_scanlines", true );
	OPENGL->set_uniform( "ub_crt_tint", true );
	OPENGL->set_uniform( "ub_chromatic_abberation", true );

	//OPENGL->set_uniform( "ub_desaturation", true );
	//OPENGL->set_uniform( "ub_inverted", true );
	//OPENGL->set_uniform( "u_color_overlay", w_color( 0.15f, 0.75f, 0.45f, 0.5f ) );
	//OPENGL->set_uniform( "u_color_tint", w_color( 0.15f, 0.75f, 0.45f, 0.25f ) );
}

void snake_pong::reset_layer_stack_to_main_menu()
{
	engine->layer_mgr->clear_stack();

	engine->layer_mgr->push<layer_background>();
	engine->layer_mgr->push<layer_main_menu>();
}

void snake_pong::new_game()
{
	w_game::new_game();

	engine->box2d_world->SetGravity( w_vec2( 0.f, 0.f ).as_b2Vec2() );
}
