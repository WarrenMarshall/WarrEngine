
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void scene_controller_required::pushed()
{
	scene::pushed();

	tex_game_controller = g_engine->find_asset<texture_asset>( "engine_game_controller" );
	anim_tween = tween( 0.0f, 1.0f, 500, tween_type::pingpong, tween_via::sinusoidal );

	save_mouse_mode();
	g_engine->window.set_mouse_mode( mouse_mode::os );
	flags.blocks_further_update = true;
}

void scene_controller_required::popped()
{
	scene::popped();

	restore_mouse_mode();
}

// ----------------------------------------------------------------------------

void draw_controller_true( ui_control* control, const rect& rc_ui )
{
	auto& anim_tween = ( (scene_controller_required*)g_engine->scenes.current_scene )->anim_tween;

	scoped_render_state;

	render::state->color = make_color( color::green, 1.0f );
	render::state->scale = 1.5f;
	render::state->angle = -10.f + ( 20.f * *anim_tween );

	render::draw_sprite( control->image, rc_ui.get_midpoint() );
}

// ----------------------------------------------------------------------------

void draw_controller_false( ui_control* control, const rect& rc_ui )
{
	auto& anim_tween = ( (scene_controller_required*)g_engine->scenes.current_scene )->anim_tween;

	scoped_render_state;

	render::state->color = make_color( color::grey, 0.25f + ( *anim_tween * 0.5f ) );
	render::state->scale = 1.0f + ( *anim_tween * 0.25f );

	render::draw_sprite( control->image, rc_ui.get_midpoint() );
}

// ----------------------------------------------------------------------------


void scene_controller_required::draw_ui()
{
	scene::draw_ui();

	/*
		{
			scoped_render_state;

			w_render::state->color = make_color( 0, 0.75f );
			w_render::draw_tiled( g_engine->find_asset_in_cache<texture_asset>( "engine_white" ), { 0.f, 0.f, ui_w, ui_h } );
		}

		float panel_w = ( viewport_w / 3.f ) * 2.f;
		float panel_h = 80.f;

		if( engine->input->gamepad )
		{
			if( msg_true.empty() )
			{
				msg_true = w_render::wrap_string_to_width(
					"Game controller detected!\n\nPress any button to continue...",
					panel_w );
			}

			panel_h += msg_true.size() * w_render::state->font->get_max_height();
		}
		else
		{
			if( msg_false.empty() )
			{
				msg_false = w_render::wrap_string_to_width(
					"This game requires a game controller but one was not detected.\n\nPlease connect one to continue.",
					panel_w );
			}

			panel_h += msg_false.size() * w_render::state->font->get_max_height();
		}

		ui->do_control<ui_panel_control>( H( "main_panel" ) )
			->set_text( engine->input->gamepad ? "CONTROLLER FOUND!" : "CONTROLLER REQUIRED" )
			->set_rect( { ui_hw - ( panel_w / 2.f ), ui_hh - ( panel_h / 2.f ), panel_w, panel_h } )
			->finalize();

		w_render::state->z += zdepth_nudge;

		if( engine->input->gamepad )
		{
			ui->do_control<ui_image_control>()
				->set_image( tex_game_controller )
				->set_size( { w_sz::ignored, tex_game_controller->rc.h } )
				->set_func_draw( draw_controller_true )
				->finalize();

			float xpos = ui->current_control->rc_client.x;
			float ypos = ui->current_control->rc_client.y + ui->current_control->rc_client.h;

			for( auto& str : msg_true )
			{
				ui->do_control<ui_label_control>()
					->set_text( str )
					->set_pos( { xpos, ypos } )
					->set_text_align( align::hcenter )
					->finalize();

				ypos += w_render::state->font->get_max_height();
			}
		}
		else
		{
			ui->do_control<ui_image_control>()
				->set_image( tex_game_controller )
				->set_size( { w_sz::ignored, tex_game_controller->rc.h } )
				->set_func_draw( draw_controller_false )
				->finalize();

			float xpos = ui->current_control->rc_client.x;
			float ypos = ui->current_control->rc_client.y + ui->current_control->rc_client.h;

			for( auto& str : msg_false )
			{
				ui->do_control<ui_label_control>()
					->set_text( str )
					->set_pos( { xpos, ypos } )
					->set_text_align( align::hcenter )
					->finalize();

				ypos += w_render::state->font->get_max_height();
			}
		}
	*/
}

bool scene_controller_required::on_input_pressed( const input_event* evt )
{
	if( scene::on_input_pressed( evt ) )
	{
		return true;
	}

	// don't allow exiting to the ESC menu from here
	if( evt->input_id == input_id::key_esc )
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

void scene_controller_required::update()
{
	// empty function so we can prevent the base class from updating and trying
	// to add multiple copies of this scene
}

}
