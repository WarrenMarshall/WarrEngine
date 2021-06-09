
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

bool scene_msg_box_ui_callback::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_enter:
		case input_id::key_esc:
		{
			g_engine->scenes.pop();
			return true;
		}
	}

	return false;
}

// ----------------------------------------------------------------------------

scene_msg_box::scene_msg_box()
{
	ui_callback = std::make_unique<scene_msg_box_ui_callback>();
}

void scene_msg_box::pushed()
{
	scene::pushed();

	blocks_further_input = true;
}

void scene_msg_box::popped()
{
	scene::popped();
}

void scene_msg_box::draw_ui()
{
	scene::draw_ui();

	{
		scoped_render_state;

		render::state->color = make_color( 0, 0.75f );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_white" ), { 0.f, 0.f, ui_w, ui_h } );
	}

	float panel_w = ui_w * .75f;
	float panel_h = 58.f;

	if( msg.empty() )
	{
		msg = render::wrap_string_to_width( g_engine->msg_box.msg, panel_w );
	}

	panel_h += msg.size() * render::state->font->get_max_height();

	rect rc_panel = { 0.f, 0.f, panel_w, panel_h };
	g_ui->layout_init( rc_panel );

	g_ui
		->panel_control()
		->set_rect( rc_panel )
		->center_control_on_screen()
		->adjust_layout_to_client_area()
		->end();

	g_ui
		->spacer_control()
		->end();

	for( auto& str : msg )
	{
		g_ui
			->label_control()
			->set_text( str )
			->set_text_align( align::hcenter )
			->end();
	}

	g_ui
		->spacer_control()
		->end();

	if( g_ui
		->button_control()
		->set_text( "OK" )
		->end() )
	{
		g_engine->scenes.pop();
	}
}

}