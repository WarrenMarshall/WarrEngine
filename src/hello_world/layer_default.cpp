
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

float layer_default_ui_callback::get_data_for_control(const w_imgui_control& control)
{
	switch (control.tag)
	{
		case H("slider_01"):
		{
			return slider_value;
		}
		break;

		case H( "slider_02" ):
		{
			return slider_value;
		}
		break;
	}

	return 0.0f;
}

void layer_default_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	auto layer = static_cast<layer_default*>( LAYER );

	switch( control.tag )
	{
		case H( "checkbox_01" ):
		{
			b_checkbox_01 = !b_checkbox_01;
			layer->glow_tween.toggle_direction();
		}
		break;

		case H( "push_button_01" ):
		{
			log( "BAM! Button_01 : {:.0f}, {:.0f}", result.click_pos.x, result.click_pos.y );
		}
		break;

		case H( "push_button_02" ):
		{
			log( "BAM! Button_02." );
		}
		break;

		case H( "slider_01" ):
		{
			slider_value = result.click_pct.x;
		}
		break;

		case H( "slider_02" ):
		{
			slider_value = result.click_pct.x;
		}
		break;
	}
}

void layer_default_ui_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	auto layer = static_cast<layer_default*>( LAYER );

	switch( control.tag )
	{
		case H( "slider_01" ):
		{
			slider_value = result.click_pct.x;
			layer->glow_intensity = slider_value * 5.0f;
		}
		break;

		case H( "slider_02" ):
		{
			slider_value = result.click_pct.x;
			if( control.interval > 0 )
			{
				slider_value = result.click_pct.x - glm::mod( result.click_pct.x, control.interval );
			}
			layer->glow_intensity = slider_value * 5.0f;
		}
		break;
	}
}

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
	glow_tween = w_tween( 0.0f, 5.0f, 2500 );
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
		->push_glow( glow_intensity )// *glow_tween )
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
		->finalize();

	IMGUI->do_divider()
		->finalize();

	IMGUI->do_label()
		->set_text( fmt::format( "Glow Intensity : {:.0f}%", imgui_callback.slider_value * 100.0f ) )
		->finalize();
	IMGUI->do_slider( H( "slider_01" ) )
		->finalize();

	IMGUI->do_divider()->finalize();

	IMGUI->do_label()
		->set_text( "Fixed Interval Slider" )
		->finalize();
	IMGUI->do_slider( H( "slider_02" ) )
		->set_interval( 0.2f )
		->finalize();

	IMGUI->do_divider()->finalize();

	IMGUI->do_edit_box( H( "edit_box_01" ) )
		->finalize();
}

w_imgui_callback* layer_default::get_imgui_callback()
{
	return &imgui_callback;
}
