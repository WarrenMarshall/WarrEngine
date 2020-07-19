
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

	MATRIX
		->push_identity()
		->translate( w_vec2( 0, 0 ) );
	RENDER
		->begin()
		->push_depth( -900 )
		->push_color( w_color( 0.2f, 0.2f, 0.2f ) )
		//->draw_sprite( engine->white_solid, w_sz( v_window_w, v_window_h ) )
		->end();
	MATRIX->pop();
}
