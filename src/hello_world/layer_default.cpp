
#include "app_header.h"

// ----------------------------------------------------------------------------

e_imgui_control_state layer_default_ui_callback::get_state_for_control( const w_imgui_control& control )
{
	switch( control.tag )
	{
		case H( "checkbox_01" ):
		{
			return b_checkbox_01 ? imgui_control_state::checked : imgui_control_state::unchecked;
		}
		break;
	}

	return w_imgui_callback::get_state_for_control( control );
}

void layer_default_ui_callback::was_left_clicked( const w_imgui_control& control )
{
	switch( control.tag )
	{
		case H( "checkbox_01" ):
		{
			b_checkbox_01 = !b_checkbox_01;
		}
		break;

		case H( "push_button_01" ):
		{
			log( "BAM! Button_01." );
		}
		break;

		case H( "push_button_02" ):
		{
			log( "BAM! Button_02." );
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
	tex_hello_world = a_texture::find( "tex_hello_world" );

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

	IMGUI->do_panel( H( "main_panel" ) )
		->set_position( { ui_canvas_hw, 32.0f } )
		->set_size( { 146.0f, 200.0f } )
		->finalize();

	IMGUI->do_push_button( H( "push_button_01" ) )
		->set_text( "Push Button" )
		->set_position( imgui_flow::last_crc_topleft )
		->finalize();

	IMGUI->do_checkbox( H( "checkbox_01" ) )
		->set_text( "Check Box" )
		->finalize();

	IMGUI->do_divider()
		->finalize();

	IMGUI->do_label( H( "caption_label" ) )
		->set_text( "Warren Rules!" )
		->set_slice_def( "simple_ui_label" )
		->set_size( { w_sz::ignore, 16.0f } )
		->finalize();
}

w_imgui_callback* layer_default::get_imgui_callback()
{
	return &imgui_callback;
}
