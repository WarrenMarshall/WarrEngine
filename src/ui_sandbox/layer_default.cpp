
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

void layer_default::draw()
{
	RENDER
		->begin()
		->draw( gradient, w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_rgb( w_color::white )
		->push_align( align::centered )
		->draw_string( "Ut enim ad minim veniam.", w_rect( v_window_hw, v_window_hh ) )
		->end();

	w_layer::draw();
}

void layer_default::draw_ui()
{
	// ----------------------------------------------------------------------------
	// panel

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { 0,0,100,ui_canvas_h } )
		->finalize();
	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { 100,0,ui_canvas_w/3.0f,ui_canvas_h/2.0f } )
		->finalize();

#if 0
	// ----------------------------------------------------------------------------
	// push button

	if(
		IMGUI->init_push_button()
		->set_label( "Lorem Ipsum" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::last_crc_topleft, { 100, 24 } )
		->finalize()
		->was_left_clicked() )
	{
		log_msg( "button clicked" );
	}

	if(
		IMGUI->init_push_button( "button_02" )
		->set_label( "Lorem Ipsum #2" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::down )
		->finalize()
		->was_left_clicked() )
	{
		log_msg( "button #2 clicked" );
	}

	// ----------------------------------------------------------------------------
	// push button / warning icon

	if(
		IMGUI->init_push_button()
		->set_subtexture( a_subtexture::find( "sub_warning_icon" ) )
		->set_rect( imgui_flow::right | imgui_flow::vcenter, { 16, 16 } )
		->finalize()
		->was_left_clicked() )
	{
		log_msg( "img button clicked" );
	}

	if(
		IMGUI->init_push_button()
		->set_last_control_from_tag( "button_02" )
		->set_label( "Lorem Ipsum #3" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::down )
		->finalize()
		->was_left_clicked() )
	{
		log_msg( "button #3 clicked" );
	}
#endif
}

void layer_default::update()
{
	w_layer::update();

	angle += 30.0f * w_time::FTS_step_value_s;
	auto e = find_entity_from_tag( "arrow_mesh" );
	e->set_angle( angle );
}
