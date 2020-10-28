
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	gradient = a_gradient::find( "background_gradient" );
	engine->window->set_mouse_mode( mouse_mode::custom);
}

void leftclick_callback()
{
	log_msg( "callback called" );
}

void layer_default::draw()
{
	static bool init_done = false;
	//static w_ui_style_button style_img_push_button;

	if( !init_done )
	{
		init_done = true;

		//style_img_push_button.slice_def = std::nullopt;

		//my_button_style.cb_was_left_clicked = leftclick_callback;
	}

	w_layer::draw();

	RENDER
		->begin()
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_rgb( w_color::white )
		->push_align( align::centered )
		->draw_string( "Ut enim ad minim veniam.", w_rect( v_window_hw, v_window_hh ) )
		->end();

	// ----------------------------------------------------------------------------
	// panel

	IMGUI->begin_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { 8,8,200,150 } )
		->end();

	IMGUI->set_last_as_origin();
	{
		// ----------------------------------------------------------------------------
		// push button

		if(
			IMGUI->begin_push_button()
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_label( "Lorem Ipsum" )
			->set_rect( { 8, 8, 100, 24 } )
			->end()
			->was_left_clicked() )
		{
			log_msg( "button clicked" );
		}

		// ----------------------------------------------------------------------------
		// push button / warning icon

		if(
			IMGUI->begin_push_button()
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_subtexture( a_subtexture::find( "sub_warning_icon" ) )
			->set_rect( { 0, 32, 40, 40 } )
			->end()
			->was_left_clicked() )
		{
			log_msg( "img button clicked" );
		}
	}
	IMGUI->clear_origin();
}
