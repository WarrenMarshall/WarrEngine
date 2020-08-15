
#include "app_header.h"

void layer_worldviewport::push()
{
}

void layer_worldviewport::draw()
{
	w_layer::draw();

	// solid background

	RENDER
		->begin()
		->push_rgb( w_color( 0.2f, 0.2f, 0.2f ) )
		->draw_filled_rectangle( w_rect( 0.0f, 0.0f, v_window_w, v_window_h ) )
		->end();
}
