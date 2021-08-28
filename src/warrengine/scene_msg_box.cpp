
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

bool Scene_Msg_Box_UI_Callback::on_input_pressed( const Input_Event* evt )
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

Scene_Msg_Box::Scene_Msg_Box()
{
	ui_callback = std::make_unique<Scene_Msg_Box_UI_Callback>();
}

void Scene_Msg_Box::pushed()
{
	scene::pushed();

	flags.blocks_further_input = true;
}

void Scene_Msg_Box::popped()
{
	scene::popped();
}

void Scene_Msg_Box::draw_ui()
{
	scene::draw_ui();

	{
		scoped_render_state;

		Render::state->color = make_color( 0, 0.75f );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_white" ), { 0.f, 0.f, ui_w, ui_h } );
	}

	float panel_w = ui_w * .75f;
	float panel_h = 58.f;

	if( msg.empty() )
	{
		msg = Render::wrap_string_to_width( g_engine->msg_box.msg, panel_w );
	}

	panel_h += msg.size() * Render::state->font->get_max_height();

	Rect rc_panel = { 0.f, 0.f, panel_w, panel_h };
	g_ui->layout_init( rc_panel );

	g_ui
		->panel_control()
		->set_rect( rc_panel )
		->center_control_on_screen()
		->adjust_layout_to_client_area()
		->done();

	g_ui
		->spacer_control()
		->done();

	for( auto& str : msg )
	{
		g_ui
			->label_control()
			->set_text( str )
			->set_text_align( align::hcenter )
			->done();
	}

	g_ui
		->spacer_control()
		->done();

	if( g_ui
		->button_control()
		->set_text( "OK" )
		->done() )
	{
		g_engine->scenes.pop();
	}
}

}