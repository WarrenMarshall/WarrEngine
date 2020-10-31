
#include "master_pch.h"
#include "master_header.h"

void layer_esc_menu::push()
{
	w_layer::push();

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

	constexpr float num_buttons = 4;
	constexpr float button_w = 140.0f;
	constexpr float button_h = 24.0f;

	float panel_w = button_w + ( UI_PADDING * 4.0f );
	float panel_h = ( button_h * num_buttons ) + ( UI_PADDING * ( num_buttons * 2.0f ) );

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "sd_ui_panel" ) )
		->set_rect( { v_window_hw - ( panel_w / 2.0f ), v_window_hh - ( panel_h / 2.0f ), panel_w, panel_h } )
		->go();

	//IMGUI->push_parent();
	{
		if( IMGUI->set_label( "Main Menu" )
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_rect( { v_window_hw - ( button_w / 2.0f ), UI_PADDING * 2, button_w, button_h } )
			->go()
			->was_left_clicked() )
		{
			base_game->reset_layer_stack_to_main_menu();
		}
	}
	//IMGUI->pop_parent();

	/*
	RENDER
		->push_rgba( w_color( 0.2f, 0.2f, 0.2f, 0.9f ) )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) );

	static float num_buttons = 4;
	static float button_w = 140.0f;
	static float button_h = 24.0f;

	static float panel_w = button_w + ( UI->theme->control_padding * 4.0f );
	static float panel_h = ( button_h * num_buttons ) + ( UI->theme->control_padding * ( num_buttons * 2.0f ) );

	w_rect rc_panel = { v_window_hw - ( panel_w / 2.0f ), v_window_hh - ( panel_h / 2.0f ), panel_w, panel_h };
	IMGUI->panel( rc_panel );

	w_rect rc_button = { v_window_hw - ( button_w / 2.0f ), rc_panel.y + (UI->theme->control_padding * 2), button_w, button_h };
	if( IMGUI->push_button( "Main Menu", rc_button ).was_left_clicked() )
	{
		base_game->reset_layer_stack_to_main_menu();
	}
	rc_button.y += button_h + UI->theme->control_padding;

	if( IMGUI->push_button( "Resume", rc_button ).was_left_clicked() )
	{
		engine->layer_mgr->pop();
	}
	rc_button.y += button_h + UI->theme->control_padding;

	if( IMGUI->push_button( "Toggle Fullscreen", rc_button ).was_left_clicked() )
	{
		engine->window->toggle_fullscreen();
	}
	rc_button.y += button_h + UI->theme->control_padding;

	if( IMGUI->push_button( "Exit To Windows", rc_button ).was_left_clicked() )
	{
		engine->is_running = false;
	}
	*/
}
