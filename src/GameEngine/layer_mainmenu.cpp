
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_mainmenu::push()
{
	//tween_logo = add_tween( std::make_shared<w_tween>( e_tween_type::half_sine, 0.f, 360.f, 270.0f ) );

	//add_particle_emitter( w_vec2( v_window_w / 2.0f, v_window_h ), "mainmenu_bubbles" );

	grad_teal_blue = engine->get_asset<a_gradient>( "teal_blue" );
	logo = engine->get_asset<a_texture>( "logo" );
	germ = engine->get_asset<a_texture>( "germ_red_idle" );
	mus_mainmenu = engine->get_asset<a_music>( "menu_mus" );
}

void layer_mainmenu::becoming_top_layer()
{
	mus_mainmenu->play();
}

void layer_mainmenu::getting_covered()
{
	mus_mainmenu->stop();
}

void layer_mainmenu::draw()
{
	//RENDER->draw_texture( grad_teal_blue, w_rect( 0.0f, 0.0f, v_window_w, v_window_h), 0.0f );
	//RENDER->draw_sprite( logo->get_texture(), w_vec2( RENDER->rcv.w / 2, 65 - ( tween_logo->get_fval() * 10.f ) ), W_COLOR_WHITE, 0.f, 0.75f );
	w_layer::draw();
	//RENDER->draw_sprite( germ->get_texture(), w_vec2( 100, 144 ), W_COLOR_WHITE, 0.f, 0.75f );
}
