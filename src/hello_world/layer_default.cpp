
#include "app_header.h"

// ----------------------------------------------------------------------------

e_imgui_control_state layer_default_ui_callback::get_state_for_control( w_imgui_control* control )
{
	if( control->tag == "checkbox_01" )
	{
		return b_checkbox_01 ? imgui_control_state::checked : imgui_control_state::unchecked;
	}

	return w_imgui_callback::get_state_for_control( control );
}

void layer_default_ui_callback::was_left_clicked( w_imgui_control* control )
{
	if( control->tag == "push_button_01" )
	{
		log( "BAM! Button_01 in the callback hizzy." );
	}
	else if( control->tag == "push_button_02" )
	{
		log( "BAM! Button_02 in the callback hizzy." );
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
	tex_hello_world = a_texture::find( "tex_hello_world" );

	ui_data_provider = &data_provider;

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
	OPENGL->init_view_matrix_identity_ui();

	RENDER
		->draw( tex_hello_world, w_rect( 16, 16 ) );

	IMGUI->init_panel()
		->set_pos( { ui_canvas_hw, 32.0f } )
		->set_size( { 146.0f, 200.0f } )
		->finalize();

	if( IMGUI->init_push_button( "push_button_01" )
		->set_label( "Push Button #1" )
		->set_pos( imgui_flow::last_crc_topleft )
		->set_size( { 130.0f, 24.0f } )
		->finalize()->was_left_clicked() )
	{
		log( "PUSH BUTTON #1 clicked" );
	}

	if( IMGUI->init_push_button( "push_button_02" )
		->set_label( "Push Button #2" )
		->set_pos( imgui_flow::down )
		->set_size( { 130.0f, 24.0f } )
		->finalize()->was_left_clicked() )
	{
		log( "PUSH BUTTON #2 clicked" );
	}

	if( IMGUI->init_checkbox( "checkbox_01" )
		->set_label( "Check Box #1" )
		->set_pos( imgui_flow::down )
		->set_size( { 130.0f, 24.0f } )
		->finalize()->was_left_clicked() )
	{
		log( "CHECK BOX #1 clicked" );
	}
}
