
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

ui_control_data* scene_esc_menu_ui_callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "check_fullscreen" ):
		{
			data_fullscreen.set_bool_value( g_engine->window.is_fullscreen );
			return &data_fullscreen;
		}
	}

	return ui_callback::get_data( tag );
}

bool scene_esc_menu_ui_callback::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_esc:
		{
			g_engine->scenes.pop();
			return true;

		}
	}

	return false;
}

// ----------------------------------------------------------------------------

scene_esc_menu::scene_esc_menu()
{
	ui_callback = std::make_unique<scene_esc_menu_ui_callback>();
}

void scene_esc_menu::pushed()
{
	scene::pushed();

	save_mouse_mode();
	g_engine->window.set_mouse_mode( mouse_mode::os );

	g_engine->pause();
}

void scene_esc_menu::popped()
{
	scene::popped();

	restore_mouse_mode();
	g_engine->resume();
}

void scene_esc_menu::draw_ui()
{
	scene::draw_ui();

	{
		scoped_render_state;

		render::state->color = make_color( 0, 0.75f );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_white" ), { 0.f, 0.f, ui_w, ui_h } );
	}

	int num_buttons = 3 + g_base_game->flags.has_main_menu;

	auto slice_def = g_engine->find_asset<slice_def_asset>( "simple_ui_panel" );

	float panel_w =
		slice_def->get_left_slice_sz()
		+ ui_button_control::get_default_width()
		+ slice_def->get_right_slice_sz();

	float panel_h =
		( ui_button_control::get_default_height() * num_buttons )
		+ slice_def->get_top_slice_sz()
		+ ( get_ui_callback()->get_control_margin( ui_control_type::button ).y * ( num_buttons - 1 ) )
		+ slice_def->get_bottom_slice_sz()
		+ 12.f;

	g_ui->layout_clear( rect( 0.f, 0.f, panel_w, panel_h ) );

	g_ui->panel_control( H( "main_panel" ) )
		->center_control_on_screen()
		->adjust_layout_to_client_area()
		->end();

	{
		render::state->z += zdepth_nudge;

		g_ui->caption_control()
			->set_text( "PAUSE MENU" )
			->end();

		g_ui->spacer_control()
			->end();

		if( g_ui->button_control( H( "button_resume" ) )
			->set_text( "Resume" )
			->end() )
		{
			g_engine->scenes.pop();
		}

		if( g_base_game->flags.has_main_menu )
		{
			if( g_ui->button_control( H( "button_main_menu" ) )
				->set_text( "Main Menu" )
				->end() )
			{
				g_engine->scenes.pop();
				g_base_game->reset_scene_stack_to_main_menu();
			}
		}

		if( g_ui->button_control( H( "button_exit" ) )
			->set_text( "Exit To Desktop" )
			->end() )
		{
			g_engine->is_running = false;
		}

		g_ui->divider_control()
			->end();

		if( g_ui->check_control( H( "check_fullscreen" ) )
			->set_text( "Full Screen?" )
			->end() )
		{
			g_engine->window.toggle_fullscreen();
		}
	}
}

}
