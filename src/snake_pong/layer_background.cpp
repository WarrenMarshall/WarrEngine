
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_background::layer_background()
{
	draws_completely_solid = true;
}

void layer_background::push()
{
	w_layer::push();

	//gradient = a_gradient::find( "background_gradient" );

	engine->window->set_mouse_mode( mouse_mode::custom );
}

void layer_background::draw()
{
	w_layer::draw();

	//RENDER
	//	->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) );
}
