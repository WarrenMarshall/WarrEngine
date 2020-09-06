
#include "app_header.h"

layer_background::layer_background()
{
	draws_completely_solid = true;
}

void layer_background::push()
{
	background_gradient = engine->get_asset<a_gradient>( "background_gradient" );

	add_entity<w_entity>( { v_window_hw, v_window_hh } )
		->add_component<ec_emitter>()->init( "background_stars" );
}

void layer_background::draw()
{
	RENDER
		->begin()
		->push_rgb( w_color::white )
		->draw( background_gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->end();

	w_layer::draw();
}
