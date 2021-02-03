
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_default_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

	switch( control.tag )
	{
		case H( "button_UI" ):
		{
		}
		break;
	}
}

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = a_texture::find( "background_gradient" );

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
	w_layer::draw_ui();

	IMGUI->do_panel( H( "main_panel" ) )
		->set_text( "Test Cases" )
		->set_position( { 16.0f, 16.0f } )
		->set_size( { ui_canvas_w - 32, ui_canvas_h - 32 } )
		->finalize();

	IMGUI->do_push_button( H( "button_UI" ) )
		->set_text( "UI Controls" )
		->set_position( imgui_flow::last_crc_topleft )
		->finalize();
}

w_imgui_callback* layer_default::get_imgui_callback()
{
	return &imgui_callback;
}
