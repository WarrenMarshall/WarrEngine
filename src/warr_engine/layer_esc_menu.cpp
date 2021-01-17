
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void layer_esc_menu_ui_callback::was_left_clicked( const w_imgui_control& control )
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

		case H( "option_fullscreen" ):
		{
			engine->window->toggle_fullscreen();
		}
		break;

		case H( "button_exit" ):
		{
			engine->is_running = false;
		}
		break;
	}
}

e_imgui_control_state layer_esc_menu_ui_callback::get_state_for_control( const w_imgui_control& control )
{
	switch( control.tag )
	{
		case H("option_fullscreen"):
		{
			return engine->window->is_fullscreen ? imgui_control_state::checked : imgui_control_state::unchecked;
		}
		break;
	}

	return w_imgui_callback::get_state_for_control( control );
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

	w_render_state_opt rso;
	rso.color = w_color::pal( 0 );
	rso.color->a = 0.75f;

	RENDER
		->push_render_state( rso )
		->draw_filled_rectangle( w_rect( 0, 0, ui_canvas_w, ui_canvas_h ) )
		->pop();

	constexpr float num_buttons = 4.0f;
	constexpr float button_w = 140.0f;
	constexpr float button_h = 24.0f;

	float panel_w = button_w + ( IMGUI->current_callback->get_control_margin() * 4.0f );
	auto slice = a_9slice_def::find( "simple_ui_panel" );
	float panel_h =
		( button_h * num_buttons )
		+ ( IMGUI->current_callback->get_control_margin() * ( num_buttons - 1 ) )
		+ slice->patches[ slicedef_patch::P_00 ]->rc.h
		+ slice->patches[ slicedef_patch::P_22 ]->rc.h;

	IMGUI->init_panel()
		->set_slice_def( a_9slice_def::find( "simple_ui_panel" ) )
		->set_position( { ui_canvas_hw - ( panel_w / 2.0f ), ui_canvas_hh - ( panel_h / 2.0f ) } )
		->set_size( { panel_w, panel_h } )
		->finalize();

	IMGUI->init_push_button( H( "button_resume" ) )
		->set_text( "Resume" )
		->set_position( imgui_flow::last_crc_topleft )
		->set_size( { IMGUI->last_control->rc_client.w, button_h } )
		->finalize();

	IMGUI->init_push_button( H( "button_main_menu" ) )
		->set_text( "Main Menu" )
		->set_position( imgui_flow::down )
		->finalize();

	IMGUI->init_checkbox( H( "option_fullscreen" ) )
		->set_text( "Full Screen?" )
		->set_position( imgui_flow::down )
		->finalize();

	IMGUI->init_push_button( H( "button_exit" ) )
		->set_text( "Exit To Windows" )
		->set_position( imgui_flow::down )
		->finalize();
}

w_imgui_callback* layer_esc_menu::get_imgui_callback()
{
	return &ui_callback;
}
