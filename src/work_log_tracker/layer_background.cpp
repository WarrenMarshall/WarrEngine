
#include "app_header.h"

layer_background::layer_background()
{
	draws_completely_solid = true;
}

void layer_background::push()
{
	background_gradient = engine->get_asset<a_gradient>( "background_gradient" );
}

void layer_background::draw()
{
	RENDER
		->draw( background_gradient, w_rect( 0, 0, v_window_w, v_window_h ) );
}
