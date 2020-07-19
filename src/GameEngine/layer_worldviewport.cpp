
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_worldviewport::push()
{
}

void layer_worldviewport::draw()
{
	w_layer::draw();

	// solid background

	RENDER
		->begin()
		->push_depth( -900 )
		->push_color( w_color( 0.2f, 0.2f, 0.25f ) )
		->draw_filled_rectangle( w_rect( 0.0f, 0.0f, v_window_w, v_window_h ) )
		->end();
}
