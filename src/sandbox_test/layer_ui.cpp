
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_ui_callback::layer_ui_callback()
{
	edit_name_data.max_length = 15;
	edit_name_data.valid_char_list = valid_chars_alphanumeric + valid_chars_simple_whitespace + valid_chars_punctuation;
	edit_email_data.max_length = 10;
}

void layer_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

	switch( control.tag )
	{
		case H( "push_button_01" ):
		{
			UI->show_msg_box( "You clicked the button!" );
		}
		break;
	}
}

void layer_ui_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_motion( control, result );

	auto layer = static_cast<layer_ui*>( IMGUI->current_layer );

	switch( control.tag )
	{
		case H( "slider_01" ):
		{
		}
		break;
	}
}

// ----------------------------------------------------------------------------

layer_ui::layer_ui()
{
	draws_completely_solid = false;
}

void layer_ui::push()
{
	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_ui::draw()
{
	w_layer::draw();
}

void layer_ui::draw_ui()
{
	w_layer::draw_ui();

	RENDER
		->push_rgba( w_color( 0.2f, 0.2f, 0.2f, 0.85f ) )
		->draw_filled_rectangle( w_rect( 0, 0, ui_canvas_w, ui_canvas_h ) );

	IMGUI
		->do_panel( H( "main_panel" ) )
		->set_text( "SAMPLE CONTROLS" )
		->set_position( { ui_canvas_hw, 32.0f } )
		->set_size( { 146.0f, 200.0f } )
		->finalize();

	IMGUI
		->do_push_button( H( "push_button_01" ) )
		->set_text( "Push Button" )
		->set_position( imgui_flow::last_crc_topleft )
		->finalize();

	IMGUI
		->do_checkbox( H( "checkbox_01" ) )
		->set_text( "Check Box" )
		->finalize( &imgui_callback.checkbox_data );

	IMGUI
		->do_divider()
		->finalize();

	IMGUI
		->do_label()
		->set_text( fmt::format( "Glow Intensity : {:.0f}%", std::get<float>( imgui_callback.slider_01_value.data ) * 100.0f ) )
		->finalize();

	IMGUI
		->do_slider( H( "slider_01" ) )
		->finalize( &imgui_callback.slider_01_value );

	IMGUI
		->do_divider()
		->finalize();

	IMGUI
		->do_label()
		->set_text( "Fixed Interval Slider" )
		->finalize();

	IMGUI
		->do_slider( H( "slider_02" ) )
		->set_interval( 0.2f )
		->finalize( &imgui_callback.slider_02_value );

	IMGUI
		->do_divider()
		->finalize();

	IMGUI
		->do_edit_box( H( "edit_name" ) )
		->finalize( &imgui_callback.edit_name_data );

	IMGUI
		->do_edit_box( H( "edit_email" ) )
		->finalize( &imgui_callback.edit_email_data );
}

w_imgui_callback* layer_ui::get_imgui_callback()
{
	return &imgui_callback;
}
