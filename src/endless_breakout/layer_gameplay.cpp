
#include "app_header.h"

layer_gameplay::layer_gameplay()
{
}

void layer_gameplay::push()
{
	///spawn_entity<e_background_stars>( { v_window_hw, v_window_hh } );
}

bool layer_gameplay::handle_input_event( const w_input_event* evt )
{

	return false;
}

void layer_gameplay::draw()
{
	w_layer::draw();
}

