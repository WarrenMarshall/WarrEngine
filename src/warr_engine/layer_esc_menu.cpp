
#include "master_pch.h"
#include "master_header.h"

void layer_esc_menu::push()
{
	w_layer::push();

	blocks_further_input = true;

	save_mouse_mode = engine->window->mouse_mode;
	engine->window->set_mouse_mode( mouse_mode::os );

	engine->set_pause( true );
}

void layer_esc_menu::pop()
{
	w_layer::pop();

	engine->window->set_mouse_mode( save_mouse_mode );

	engine->set_pause( false );
}

void layer_esc_menu::draw_ui()
{
	w_layer::draw_ui();

	RENDER
		->push_rgb( w_color::pal( 0 ) )
		->push_alpha( 0.75f )
		->draw_filled_rectangle( w_rect( 0, 0, ui_canvas_w, ui_canvas_h ) )
		->pop_alpha();

	constexpr float num_buttons = 4;
	constexpr float button_w = 140.0f;
	constexpr float button_h = 24.0f;

	float panel_w = button_w + ( UI_PADDING * 4.0f );
	float panel_h = ( button_h * num_buttons ) + ( UI_PADDING * ( num_buttons + 6 ) );

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { ui_canvas_hw - ( panel_w / 2.0f ), ui_canvas_hh - ( panel_h / 2.0f ), panel_w, panel_h } )
		->finalize();

	if( IMGUI->init_push_button()
		->set_label( "Resume" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::last_crc_topleft, { IMGUI->last_control->crc.w, button_h } )
		->finalize()
		->was_left_clicked() )
	{
		engine->layer_mgr->pop();
	}

	if( IMGUI->init_push_button()
		->set_label( "Main Menu" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::down )
		->finalize()
		->was_left_clicked() )
	{
		base_game->reset_layer_stack_to_main_menu();
	}

	if( IMGUI->init_push_button()
		->set_label( "Toggle Fullscreen" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::down )
		->finalize()
		->was_left_clicked() )
	{
		engine->window->toggle_fullscreen();
	}

	if( IMGUI->init_push_button()
		->set_label( "Exit To Windows" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( imgui_flow::down )
		->finalize()
		->was_left_clicked() )
	{
		engine->is_running = false;
	}
}

bool layer_esc_menu::iir_on_pressed( const w_input_event* evt )
{
	//if( evt->input_id == input_id::key_esc )
	//{
	//	engine->layer_mgr->pop();
	//	return true;
	//}

	return false;
}
