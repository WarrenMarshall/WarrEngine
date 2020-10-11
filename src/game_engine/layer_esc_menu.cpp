
#include "master_pch.h"
#include "master_header.h"

void layer_esc_menu::push()
{
	w_layer::push();

	save_mouse_mode = engine->window->mouse_mode;
	engine->window->set_mouse_mode( mouse_mode::normal );

	engine->set_pause( true );
}

void layer_esc_menu::pop()
{
	w_layer::pop();

	engine->window->set_mouse_mode( save_mouse_mode );

	engine->set_pause( false );
}

void layer_esc_menu::draw()
{
	OPENGL->init_view_matrix_identity();

	w_layer::draw();

	RENDER
		->begin()
		->push_rgba( w_color( 0.2f, 0.2f, 0.2f, 0.9f ) )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) )
		->end();

	static w_ui_style_panel style_panel;
	style_panel.base_attrib.color = w_color::dark_grey;

	static w_ui_style_button style_button;
	style_button.base_attrib.color = w_color::dark_teal;
	style_button.label_attrib.color = w_color::white;

	static float num_buttons = 4;
	static float button_w = 112.0f;
	static float button_h = 20.0f;

	static float panel_w = button_w + ( UI->theme->control_padding * 4.0f );
	static float panel_h = ( button_h * num_buttons ) + ( UI->theme->control_padding * ( num_buttons * 2.0f ) );

	w_rect rc_panel = { v_window_hw - ( panel_w / 2.0f ), v_window_hh - ( panel_h / 2.0f ), panel_w, panel_h };

	UI->im_passive( rc_panel, style_panel );

	w_rect rc_button = { v_window_hw - ( button_w / 2.0f ), rc_panel.y + (UI->theme->control_padding * 2), button_w, button_h };

	if( UI->im_active( "Main Menu", rc_button, style_button ) & im_result::left_clicked )
	{
		base_game->return_to_main_menu();
	}
	rc_button.y += button_h + UI->theme->control_padding;

	if( UI->im_active( "Resume", rc_button, style_button ) & im_result::left_clicked )
	{
		engine->layer_mgr->pop();
	}
	rc_button.y += button_h + UI->theme->control_padding;

	if( UI->im_active( "Toggle Fullscreen", rc_button, style_button ) & im_result::left_clicked )
	{
		engine->window->toggle_fullscreen();
	}
	rc_button.y += button_h + UI->theme->control_padding;

	if( UI->im_active( "Exit To Windows", rc_button, style_button ) & im_result::left_clicked )
	{
		engine->is_running = false;
	}

	OPENGL->init_view_matrix();
}
