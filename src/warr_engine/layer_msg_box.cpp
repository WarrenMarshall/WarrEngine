
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
}

void layer_msg_box::pop()
{
	w_layer::pop();
}

void layer_msg_box::draw_ui()
{
	w_layer::draw_ui();

	a_texture* tiling_background = a_texture::find( "engine_tile_background_stripe" );
	w_rect rc = w_rect( 0, 0, ui_canvas_w, ui_canvas_h );

	w_render_state_opt rso;
	rso.color = w_color::pal( 0 );
	rso.color->a = 0.25f;
	rso.uv_tiling = w_vec2::get_uv_tiling( tiling_background, rc ) / 2.0f;

	RENDER
		->push_render_state( rso )
		->draw( tiling_background, rc )
		->pop();

	IMGUI->do_panel( H( "main_panel" ) )
		->set_position( { 0.0f, ui_canvas_hh - 32.0f } )
		->set_size( { ui_canvas_w, 68.0f } )
		->finalize();

	w_rect rc_client_main_panel = IMGUI->last_rc_client;

	IMGUI
		->do_spacer()
		->set_position( imgui_flow::last_crc_topleft )
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
