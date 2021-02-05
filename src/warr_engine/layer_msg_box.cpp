
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
		}
		break;
	}

	return false;
}

// ----------------------------------------------------------------------------

void layer_msg_box::push()
{
	w_layer::push();

	blocks_further_input = true;

	//engine->pause();
}

void layer_msg_box::pop()
{
	w_layer::pop();

	//engine->resume();
}

void layer_msg_box::draw_ui()
{
	w_layer::draw_ui();

	w_render_state_opt rso;
	rso.color = w_color::pal( 0 );
	rso.color->a = 0.75f;

	RENDER
		->push_render_state( rso )
		->draw_filled_rectangle( w_rect( 0, 0, ui_canvas_w, ui_canvas_h ) )
		->pop();

	IMGUI->do_panel( H( "main_panel" ) )
		->set_position( { 0.0f, ui_canvas_hh } )
		->set_size( { ui_canvas_w, ui_canvas_h } )
		->finalize();

	w_rect rc_win_panel = IMGUI->last_rc_win;

	IMGUI
		->do_label()
		->set_position( imgui_flow::last_crc_topleft )
		->set_size( { rc_win_panel.w, w_sz::ignore } )
		->set_text( UI->msg_box.msg )
		->set_text_align( align::hcenter )
		->finalize();

	IMGUI->do_push_button( H( "button_OK" ) )
		->set_text( "OK" )
		->finalize();

}

w_imgui_callback* layer_msg_box::get_imgui_callback()
{
	return &ui_callback;
}
