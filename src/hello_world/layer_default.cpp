
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = a_gradient::find( "background_gradient" );
	engine->window->set_mouse_mode( mouse_mode::custom );
}

void layer_default::draw()
{
	w_layer::draw();

	RENDER
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) );
}

void layer_default::draw_ui()
{
	IMGUI->init_panel()
		->set_label( "Hello, World!" )
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { 0,ui_canvas_hh - 16,ui_canvas_w,64 } )
		->finalize();

	//RENDER
	//	->begin()
	//	->push_rgb( w_color::white )
	//	->push_align( align::centered )
	//	->draw_string( engine->pixel_font, "Hello, World!", w_rect( v_window_hw, v_window_hh ) )
	//	->end();
}
