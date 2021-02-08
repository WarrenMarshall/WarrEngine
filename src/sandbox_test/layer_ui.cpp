
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_ui_callback::layer_ui_callback()
{
	edit_text_01_data.max_length = 15;
	edit_text_01_data.valid_char_list = valid_chars_alphanumeric + valid_chars_simple_whitespace + valid_chars_punctuation;
	edit_text_02_data.max_length = 10;
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

		case H( "checkbox_01" ):
		{
			if( std::get<bool>( checkbox_data.data ) )
			{
				engine->window->set_mouse_mode( mouse_mode::custom );
			}
			else
			{
				engine->window->set_mouse_mode( mouse_mode::os );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------

layer_ui::layer_ui()
{
	draws_completely_solid = true;
}

void layer_ui::push()
{
	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_ui::draw()
{
	RS->color = w_color::dark_teal;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h ) );

	RENDER_BLOCK
	(
		RS->align = align::centered;
		RS->scale = 2.0f;
		RS->color = w_color::white;
		RENDER->draw_string( "UI Controls", { ui_window_w / 2.0f, 16.0f } );
	)

	w_layer::draw();
}

void layer_ui::draw_ui()
{
	w_layer::draw_ui();

	IMGUI
		->do_panel( H( "main_panel" ) )
		->set_text( "SAMPLE CONTROLS" )
		->set_position( { ui_window_hw - 73.0f, ui_window_hh - 85.0f } )
		->set_size( { 146.0f, 200.0f } )
		->finalize();

	IMGUI
		->do_push_button( H( "push_button_01" ) )
		->set_text( "Push Button" )
		->set_position( imgui_flow::last_crc_topleft )
		->finalize();

	IMGUI
		->do_checkbox( H( "checkbox_01" ) )
		->set_text( "Custom Cursor" )
		->finalize( &imgui_callback.checkbox_data );

	IMGUI
		->do_divider()
		->finalize();

	IMGUI
		->do_label()
		->set_text( fmt::format( "Slider Value : {:.0f}%", std::get<float>( imgui_callback.slider_01_value.data ) * 100.0f ) )
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
		->do_edit_box( H( "edit_text_01" ) )
		->finalize( &imgui_callback.edit_text_01_data );

	IMGUI
		->do_edit_box( H( "edit_text_02" ) )
		->finalize( &imgui_callback.edit_text_02_data );
}

w_imgui_callback* layer_ui::get_imgui_callback()
{
	return &imgui_callback;
}
