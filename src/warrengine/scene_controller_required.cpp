
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Scene_Controller_Required::pushed()
{
	scene::pushed();

	tex_game_controller = g_engine->find_asset<Texture_Asset>( "engine_game_controller" );
	anim_tween = Tween( 0.f, 1.f, 500, tween_type::pingpong, tween_via::sinusoidal );

	save_mouse_mode();
	g_engine->window.set_mouse_mode( mouse_mode::os );
	flags.blocks_further_update = true;
}

void Scene_Controller_Required::popped()
{
	scene::popped();

	restore_mouse_mode();
}

// ----------------------------------------------------------------------------


f_decl_draw_control( draw_controller_true )
{
	auto& anim_tween = ( (Scene_Controller_Required*)g_engine->scenes.current_scene )->anim_tween;

	scoped_render_state;

	Render::state->color = make_color( Color::green, 1.f );
	Render::state->angle = -10.f + ( 20.f * *anim_tween );

	Render::draw_sprite( control->image, rc_ui.get_midpoint() );
}

// ----------------------------------------------------------------------------

f_decl_draw_control( draw_controller_false )
{
	auto& anim_tween = ( (Scene_Controller_Required*)g_engine->scenes.current_scene )->anim_tween;

	scoped_render_state;

	Render::state->color = make_color( Color::grey, 0.25f + ( *anim_tween * 0.5f ) );
	Render::state->scale = 1.f + ( *anim_tween * 0.15f );

	Render::draw_sprite( control->image, rc_ui.get_midpoint() );
}

// ----------------------------------------------------------------------------


void Scene_Controller_Required::draw_ui()
{
	scene::draw_ui();

	{
		scoped_render_state;

		Render::state->color = make_color( 0, 0.75f );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_white" ), { 0.f, 0.f, ui_w, ui_h } );
	}

	float panel_w = ( ui_w / 3.f ) * 2.f;
	float panel_h = 80.f;	// the height of the panel without the text lines. the text height is added below.

	if( g_engine->input.gamepad )
	{
		if( msg_true.empty() )
		{
			msg_true = Render::wrap_string_to_width(
				"Game controller detected!\n\nPress any button to continue...",
				panel_w );
		}

		panel_h += msg_true.size() * Render::state->font->get_max_height();
	}
	else
	{
		if( msg_false.empty() )
		{
			msg_false = Render::wrap_string_to_width(
				"This game requires a game controller but one was not detected.\n\nPlease connect one to continue.",
				panel_w );
		}

		panel_h += msg_false.size() * Render::state->font->get_max_height();
	}

	Rect rc_panel = { ui_hw - ( panel_w / 2.f ), ui_hh - ( panel_h / 2.f ), panel_w, panel_h };
	g_ui->layout_init( rc_panel );

	g_ui->panel_control( H( "main_panel" ) )
		->set_rect( rc_panel )
		->done();

	g_ui->caption_control()
		->set_text( g_engine->input.gamepad ? "Controller Found!" : "Controller Required" )
		->done();

	if( g_engine->input.gamepad )
	{
		g_ui->image_control()
			->set_image( tex_game_controller )
			->cut_top( tex_game_controller->rc.h )
			->set_func_draw( draw_controller_true )
			->done();

		for( auto& str : msg_true )
		{
			g_ui->label_control ( )
				->set_text( str )
				->cut_top( Render::state->font->get_max_height() )
				->set_text_align( align::hcenter )
				->done();
		}
	}
	else
	{
		g_ui->spacer_control()->done();

		g_ui->image_control()
			->set_image( tex_game_controller )
			->cut_top( tex_game_controller->rc.h )
			->set_func_draw( draw_controller_false )
			->done();

		for( auto& str : msg_false )
		{
			g_ui->label_control()
				->set_text( str )
				->cut_top( Render::state->font->get_max_height() )
				->set_text_align( align::hcenter )
				->done();
		}
	}
}

bool Scene_Controller_Required::on_input_pressed( const Input_Event* evt )
{
	if( scene::on_input_pressed( evt ) )
	{
		return true;
	}

	// any input from the controller will dismiss this screen
	if( evt->input_id == input_id::gamepad_button_a
		or evt->input_id == input_id::gamepad_button_b
		or evt->input_id == input_id::gamepad_button_x
		or evt->input_id == input_id::gamepad_button_y
		or evt->input_id == input_id::gamepad_button_back
		or evt->input_id == input_id::gamepad_button_dpad_down
		or evt->input_id == input_id::gamepad_button_dpad_left
		or evt->input_id == input_id::gamepad_button_dpad_right
		or evt->input_id == input_id::gamepad_button_dpad_up
		or evt->input_id == input_id::gamepad_button_left_shoulder
		or evt->input_id == input_id::gamepad_button_left_thumb
		or evt->input_id == input_id::gamepad_button_right_shoulder
		or evt->input_id == input_id::gamepad_button_right_thumb
		or evt->input_id == input_id::gamepad_button_start
		or evt->input_id == input_id::gamepad_left_stick
		or evt->input_id == input_id::gamepad_left_trigger
		or evt->input_id == input_id::gamepad_right_stick
		or evt->input_id == input_id::gamepad_right_trigger )
	{
		g_engine->scenes.pop();
		return true;
	}

	return false;
}

void Scene_Controller_Required::update()
{
	// empty function so we can prevent the base class from updating and trying
	// to add multiple copies of this scene
}

}
