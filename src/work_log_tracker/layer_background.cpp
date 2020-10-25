
#include "app_header.h"

layer_background::layer_background()
{
	draws_completely_solid = true;

	auto e = add_entity<w_entity>();
	e->set_position( { v_window_hw, 0.0f } );
	e->add_component<ec_emitter>()->init( "background_fire_down" );
	e->add_component<ec_emitter>()->init( "background_fire_up" )
		->set_transform( { 0.0f, v_window_h }, 0.0f, 1.0f );
}

void layer_background::push()
{
	background_gradient = a_gradient::find( "background_gradient" );
}

void layer_background::draw()
{
	RENDER
		->draw( background_gradient, w_rect( 0, 0, v_window_w, v_window_h ) );

	w_layer::draw();
}
