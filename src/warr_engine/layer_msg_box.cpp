
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void layer_msg_box_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	switch( control.tag )
	{
		case H( "button_OK" ):
		{
			engine->layer_mgr->pop();
		}
		break;
	}
}

bool layer_msg_box_ui_callback::on_input_pressed( const w_input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_enter:
		case input_id::key_esc:
		{
			engine->layer_mgr->pop();
			break;
		}
	}

	return false;
}

// ----------------------------------------------------------------------------

void layer_msg_box::push()
{
	w_layer::push();

	blocks_further_input = true;
}

void layer_msg_box::pop()
{
	w_layer::pop();
}

void layer_msg_box::draw_ui()
{
	w_layer::draw_ui();

	a_texture* tiling_background = a_texture::find( "engine_white" );
	w_rect rc = w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h );

	w_render_state_opt rso;
	rso.color = w_color::pal( 0 );
	rso.color->a = 0.75f;

	{
		scoped_render_state;

		render_state.set_from_opt( rso );
		w_render::draw_tiled( tiling_background, rc );
	}

	IMGUI->do_panel( H( "main_panel" ) )
		->set_pos( { 0.0f, ui_window_hh - 32.0f } )
		->set_size( { ui_window_w, 68.0f } )
		->finalize();

	w_rect rc_client_main_panel = IMGUI->last_rc_client;

	IMGUI
		->do_spacer()
		->set_pos( imgui_flow::last_crc_topleft )
		->finalize();

	IMGUI
		->do_label()
		->set_size( { rc_client_main_panel.w, w_sz::ignore } )
		->set_text( UI->msg_box.msg )
		->set_text_align( align::hcenter )
		->finalize();

	IMGUI
		->do_spacer()
		->finalize();

	IMGUI->do_push_button( H( "button_OK" ) )
		->set_text( "OK" )
		->set_align( rc_client_main_panel, align::hcenter )
		->finalize();
}

w_imgui_callback* layer_msg_box::get_imgui_callback()
{
	return &ui_callback;
}
