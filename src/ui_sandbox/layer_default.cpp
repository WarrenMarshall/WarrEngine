
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

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { 8,8,200,150 } )
		->go();

	//IMGUI->push_parent();
	{
		// ----------------------------------------------------------------------------
		// push button

		if(
			IMGUI->init_push_button()
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_label( "Lorem Ipsum" )
			->set_rect( { 0, 0, 100, 24 } )
			->go()
			->was_left_clicked() )
		{
			log_msg( "button clicked" );
		}

		if(
			IMGUI->init_push_button()
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_label( "Lorem Ipsum #2" )
			->set_rect( imgui_flow::down )
			->go()
			->was_left_clicked() )
		{
			log_msg( "button #2 clicked" );
		}


#if 0
		// ----------------------------------------------------------------------------
		// push button / warning icon

		if(
			IMGUI->init_push_button()
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_subtexture( a_subtexture::find( "sub_warning_icon" ) )
			->set_rect( { 0, 0, 40, 40 } )
			->go()
			->was_left_clicked() )
		{
			log_msg( "img button clicked" );
		}
#endif
	}
	//IMGUI->pop_parent();
}
