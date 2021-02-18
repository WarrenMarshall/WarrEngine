
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

			break;
		}

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

			break;
		}
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

	{
		add_camera();
	}
}

void layer_ui::draw()
{
	render_state = {
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, v_window_w, v_window_h ) );


	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_ui::draw_ui()
{
	w_layer::draw_ui();

	{
		scoped_imgui_location_offset( w_vec2( ( ui_window_w - 146.0f ) / 2.0f, ui_window_hh - 85.0f ) );

		IMGUI
			->do_panel( H( "main_panel" ) )
			->set_text( "SAMPLE CONTROLS" )
			->set_size( { 146.0f, 200.0f } )
			->finalize();

		IMGUI
			->do_push_button( H( "push_button_01" ) )
			->set_text( "Push Button" )
			->set_pos( imgui_flow::last_crc_topleft )
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

	{
		scoped_render_state;

		render_state = {
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "UI Controls", { ui_window_w / 2.0f, 16.0f } );

		render_state = {
			.align = align::hcenter,
			.color = w_color::light_grey,
			.scale = 1.0f
		};

		w_render::draw_string( "R_DRAG / M_DRAG - move/rotate camera", w_pos( ui_window_hw, 200.0f ) );
	}
}

w_imgui_callback* layer_ui::get_imgui_callback()
{
	return &imgui_callback;
}

bool layer_ui::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			get_camera()->get_transform()->add_pos_delta( evt->vdelta );

			return true;
		}

		if( INPUT->get_button_state( input_id::mouse_button_middle ) == button_state::held )
		{
			get_camera()->get_transform()->add_angle_delta( evt->vdelta.x );

			return true;
		}
	}

	return false;
}
