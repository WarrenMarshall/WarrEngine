
#include "master_pch.h"
#include "master_header.h"

void layer_esc_menu::push()
{
	w_layer::push();

	engine->set_pause( true );
}

void layer_esc_menu::pop()
{
	w_layer::pop();

	engine->set_pause( false );
}

void layer_esc_menu::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgba( w_color( 0.2f, 0.2f, 0.2f, 0.9f ) )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) )
		->end();

	static w_ui_style_panel style_panel;
	style_panel.base_attrib.color = w_color::dark_teal;

	static w_ui_style_button style_button;
	style_button.base_attrib.color = w_color::teal;
	style_button.label_attrib.color = w_color::white;

	constexpr float panel_w = 100.0f;
	constexpr float panel_h = 84.0f;
	w_rect rc_panel = { v_window_hw - ( panel_w / 2.0f ) , v_window_hh - ( panel_h / 2.0f ) , panel_w , panel_h };

	UI->im_passive( rc_panel, style_panel );

	constexpr float button_w = 64.0f;
	constexpr float button_h = 24.0f;

	w_rect rc_button = { v_window_hw - ( button_w / 2.0f ), rc_panel.y + 16, button_w, button_h };

	// #todo	- this syntax feels clunky. would be nice to do : ()->was_left_clicked(), or something similar
	//			- that may require a whole enum refactor to make that work universally(?)

	if( UI->im_active( "Resume", rc_button, style_button ) & im_result::left_clicked )
	{
		engine->layer_mgr->pop();
	}

	rc_button.y += button_h + UI->theme->control_padding;
	if( UI->im_active( "Exit", rc_button, style_button ) & im_result::left_clicked )
	{
		engine->is_running = false;
	}
}
