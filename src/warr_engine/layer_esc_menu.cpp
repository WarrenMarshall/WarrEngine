
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void layer_esc_menu_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	switch( control.tag )
	{
		case H( "button_resume" ):
		{
			engine->layer_mgr->pop();
		}
		break;

		case H( "button_main_menu" ):
		{
			base_game->reset_layer_stack_to_main_menu();
		}
		break;

		case H( "button_exit" ):
		{
			engine->is_running = false;
		}
		break;

		case H( "option_fullscreen" ):
		{
			engine->window->toggle_fullscreen();
		}
		break;
	}
}

w_imgui_control_data layer_esc_menu_ui_callback::get_data_for_control( const w_imgui_control& control )
{
	switch( control.tag )
	{
		case H( "option_fullscreen" ):
		{
			return w_imgui_control_data( engine->window->is_fullscreen );
		}
		break;
	}

	return w_imgui_callback::get_data_for_control( control );
}

bool layer_esc_menu_ui_callback::on_input_pressed( const w_input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_esc:
		{
			engine->layer_mgr->pop();
		}
		break;
	}

	return false;
}

// ----------------------------------------------------------------------------

void layer_esc_menu::push()
{
	w_layer::push();

	blocks_further_input = true;

	save_mouse_mode = engine->window->mouse_mode;
	engine->window->set_mouse_mode( mouse_mode::os );

	window_top_tween = w_tween( 0.0f, 150.0f, 500, tween_type::one_shot, tween_via::circular_in );
	window_top_tween.set_backwards();

	//engine->set_pause( true );
}

void layer_esc_menu::pop()
{
	w_layer::pop();

	engine->window->set_mouse_mode( save_mouse_mode );

	//engine->set_pause( false );
}

void layer_esc_menu::draw_ui()
{
	w_layer::draw_ui();

	w_render_state_opt rso;
	rso.color = w_color::pal( 0 );
	rso.color->a = 0.75f;

	RENDER
		->push_render_state( rso )
		->draw_filled_rectangle( w_rect( 0, 0, ui_canvas_w, ui_canvas_h ) )
		->pop();

	constexpr float num_buttons = 4.0f;
	float button_w = get_imgui_callback()->get_default_width( imgui_control_type::push_button );
	float button_h = get_imgui_callback()->get_default_height( imgui_control_type::push_button );

	auto slice_def = a_9slice_def::find( "simple_ui_panel" );

	float panel_w =
		slice_def->get_left_slice_sz()
		+ button_w
		+ slice_def->get_right_slice_sz();

	float panel_h =
		( button_h * num_buttons )
		+ slice_def->get_top_slice_sz()
		+ ( get_imgui_callback()->get_control_margin() * ( num_buttons - 1 ) )
		+ slice_def->get_bottom_slice_sz()
		+ 12.0f;

	IMGUI->do_panel( H( "main_panel" ) )
		->set_text( "PAUSE MENU" )
		->set_position( { ui_canvas_hw - ( panel_w / 2.0f ), ui_canvas_hh - ( panel_h / 2.0f ) - *window_top_tween } )
		->set_size( { panel_w, panel_h } )
		->finalize();

	IMGUI->do_push_button( H( "button_resume" ) )
		->set_text( "Resume" )
		->set_position( imgui_flow::last_crc_topleft )
		->set_size( { w_sz::ignore, button_h } )
		->finalize();

	IMGUI->do_push_button( H( "button_main_menu" ) )
		->set_text( "Main Menu" )
		->finalize();

	IMGUI->do_push_button( H( "button_exit" ) )
		->set_text( "Exit To Windows" )
		->finalize();

	IMGUI->do_divider()
		->finalize();

	IMGUI->do_checkbox( H( "option_fullscreen" ) )
		->set_text( "Full Screen?" )
		->finalize();

}

w_imgui_callback* layer_esc_menu::get_imgui_callback()
{
	return &ui_callback;
}
