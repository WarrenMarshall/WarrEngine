
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default_ui_callback::layer_default_ui_callback()
{
	edit_name_data.max_length = 15;
	edit_name_data.valid_char_list = valid_chars_alphanumeric;
	*edit_name_data.valid_char_list += valid_chars_simple_whitespace;
	*edit_name_data.valid_char_list += valid_chars_punctuation;
	edit_email_data.max_length = 10;
}

void layer_default_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

	switch( control.tag )
	{
		case H( "push_button_01" ):
		{
			log( "BAM! Button_01 : {:.0f}, {:.0f}", result.click_pos.x, result.click_pos.y );
		}
		break;
	}
}

void layer_default_ui_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_motion( control, result );

	auto layer = static_cast<layer_default*>( IMGUI->current_layer );

	switch( control.tag )
	{
		case H( "slider_01" ):
		{
			layer->glow_intensity = std::get<float>( slider_01_value.data ) * 5.0f;
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
	w_layer::draw_ui();

	RENDER
		->push_glow( glow_intensity )
		->draw( tex_hello_world, w_rect( 16, 24 ) )
		->push_glow( 0.0f );

	IMGUI->do_panel( H( "main_panel" ) )
		->set_text( "SAMPLE CONTROLS" )
		->set_position( { ui_canvas_hw, 32.0f } )
		->set_size( { 146.0f, 200.0f } )
		->finalize();

	IMGUI->do_push_button( H( "push_button_01" ) )
		->set_text( "Push Button" )
		->set_position( imgui_flow::last_crc_topleft )
		->finalize();

	IMGUI->do_checkbox( H( "checkbox_01" ) )
		->set_text( "Check Box" )
		->finalize( &imgui_callback.checkbox_data );

	IMGUI->do_divider()
		->finalize();

	IMGUI->do_label()
		->set_text( fmt::format( "Glow Intensity : {:.0f}%", std::get<float>( imgui_callback.slider_01_value.data ) * 100.0f ) )
		->finalize();
	IMGUI->do_slider( H( "slider_01" ) )
		->finalize( &imgui_callback.slider_01_value );

	IMGUI->do_divider()->finalize();

	IMGUI->do_label()
		->set_text( "Fixed Interval Slider" )
		->finalize();
	IMGUI->do_slider( H( "slider_02" ) )
		->set_interval( 0.2f )
		->finalize( &imgui_callback.slider_02_value );

	IMGUI->do_divider()->finalize();

	IMGUI->do_edit_box( H( "edit_name" ) )
		->finalize( &imgui_callback.edit_name_data );

	IMGUI->do_edit_box( H( "edit_email" ) )
		->finalize( &imgui_callback.edit_email_data );
}

w_imgui_callback* layer_default::get_imgui_callback()
{
	return &imgui_callback;
}

bool layer_default::on_input_pressed( const w_input_event* evt )
{
	if( !w_layer::on_input_pressed( evt ) )
	{
		log( "LAYER CLICKED!" );
	}

	return false;
}
