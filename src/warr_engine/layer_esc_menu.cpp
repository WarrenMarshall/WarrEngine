
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void layer_esc_menu_ui_callback::was_left_clicked( w_imgui_control* control )
{
	if( control->tag == "button_resume" )
	{
		engine->layer_mgr->pop();
	}
	else if( control->tag == "button_main_menu" )
	{
		base_game->reset_layer_stack_to_main_menu();
	}
	else if( control->tag == "button_fullscreen" )
	{
		engine->window->toggle_fullscreen();
	}
	else if( control->tag == "button_exit" )
	{
		engine->is_running = false;
	}
}

// ----------------------------------------------------------------------------

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
		->pop()
		->pop();

	constexpr float num_buttons = 4;
	constexpr float button_w = 140.0f;
	constexpr float button_h = 24.0f;

	float panel_w = button_w + ( UI_PADDING * 4.0f );
	float panel_h = ( button_h * num_buttons ) + ( UI_PADDING * ( num_buttons + 6 ) );

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "simple_ui_panel" ) )
		->set_position( { ui_canvas_hw - ( panel_w / 2.0f ), ui_canvas_hh - ( panel_h / 2.0f ) } )
		->set_size( { panel_w, panel_h } )
		->finalize();

	IMGUI->init_push_button( "button_resume" )
		->set_label( "Resume" )
		->set_position( imgui_flow::last_crc_topleft )
		->set_size( { IMGUI->last_control->crc.w, button_h } )
		->finalize();

	IMGUI->init_push_button( "button_main_menu" )
		->set_label( "Main Menu" )
		->set_position( imgui_flow::down )
		->finalize();

	IMGUI->init_push_button( "button_fullscreen" )
		->set_label( "Toggle Fullscreen" )
		->set_position( imgui_flow::down )
		->finalize();

	IMGUI->init_push_button( "button_exit" )
		->set_label( "Exit To Windows" )
		->set_position( imgui_flow::down )
		->finalize();
}

w_imgui_callback* layer_esc_menu::get_imgui_callback()
{
	return &ui_callback;
}
