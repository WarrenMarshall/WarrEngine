
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
		->push_rgb( w_color::white )
		->push_align( align::centered )
		//->draw_string( engine->pixel_font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.", w_rect( v_window_hw, v_window_hh ) )
		->draw_string( engine->pixel_font, "Ut enim ad minim veniam.", w_rect( v_window_hw, v_window_hh ) )
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
