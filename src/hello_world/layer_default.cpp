
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = a_gradient::find( "background_gradient" );
	subtex = a_texture::find( "tex_hello_world" )->get_subtexture();

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_default::draw()
{
	w_layer::draw();

	RENDER
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) );
}

void layer_default::draw_ui()
{
	RENDER
		->draw( subtex, w_rect( 16, 16 ) );

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { ui_canvas_hw,ui_canvas_hh,100,100 } )
		->finalize();
}
