
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = a_texture::find( "background_gradient" );
	tex_hello_world = a_texture::find( "tex_hello_world" );
	movement_tween = w_tween( -175, 175, 2000, tween_type::pingpong, tween_via::sinusoidal );
	scale_tween = w_tween( 1.0f, 5.0f, 4750, tween_type::pingpong, tween_via::quadratic );
	tilt_tween = w_tween( -25.0f, 25.0f, 10000, tween_type::pingpong, tween_via::cubic );

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_default::draw()
{
	w_layer::draw();

	RENDER
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_scale( *scale_tween )
		->push_angle( *tilt_tween )
		->draw_sprite( tex_hello_world, { v_window_hw + *movement_tween, v_window_hh } );
}
