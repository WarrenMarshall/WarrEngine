
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = a_gradient::find( "background_gradient" );
}

void layer_default::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_rgb( w_color::white )
		->push_align( align::centered )
		->draw_string( "Ut enim ad minim veniam.", w_rect( v_window_hw, v_window_hh ) )
		->end();

	// ----------------------------------------------------------------------------
	// panel

	IMGUI->panel( w_rect( 8, 8, 200, 150 ) );
	IMGUI->set_origin( { 8, 8 } );
	{
		// ----------------------------------------------------------------------------
		// push button

		if( IMGUI->push_button( "Lorem Ipsum", w_rect( 8, 8, 100, 24 ) ).was_left_clicked() )
		{
			log_msg( "button clicked" );
		}
	}
	IMGUI->clear_origin();

}
