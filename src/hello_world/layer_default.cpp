
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = engine->get_asset<a_gradient>( "background_gradient" );
}

void layer_default::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_rgb( { 1.0f, 0.5f, 0.0f } )
		->push_align( align::centered )
		->draw_string( engine->pixel_font, "Hello, World!", w_rect( v_window_hw, v_window_hh ) )
		->end();
}
