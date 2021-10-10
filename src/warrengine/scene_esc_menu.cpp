
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

UI_Control_Data* Scene_Esc_Menu_UI_Callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "check_fullscreen" ):
		{
			data_fullscreen.set_bool_value( g_engine->window.is_fullscreen );
			return &data_fullscreen;
		}
	}

	return UI_Callback::get_data( tag );
}

bool Scene_Esc_Menu_UI_Callback::on_input_pressed( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::key_esc:
		{
			g_engine->scene_mgr.pop();
			return true;

		}
	}

	return false;
}

// ----------------------------------------------------------------------------

Scene_Esc_Menu::Scene_Esc_Menu()
{
	ui_callback = &uic;
	flags.blocks_further_input = true;
	flags.blocks_further_update = true;
}

void Scene_Esc_Menu::pushed()
{
	Scene::pushed();

	save_mouse_mode();
	g_engine->window.set_mouse_mode( e_mouse_mode::os );
}

void Scene_Esc_Menu::popped()
{
	Scene::popped();

	restore_mouse_mode();
}

void Scene_Esc_Menu::draw_ui()
{
	Scene::draw_ui();

	{
		scoped_render_state;

		Render::state->color = make_color( e_pal::darkest, 0.75f );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_white" ), { 0.f, 0.f, ui_w, ui_h } );
		Render::flush_buffers();
	}

	int32_t num_buttons = 3 + ( g_base_game->flags.has_main_menu and !g_engine->scene_mgr.get_under()->flags.is_main_menu );

	auto slice_def = g_engine->find_asset<Slice_Def_Asset>( "simple_ui_panel" );

	float_t panel_w =
		slice_def->get_left_slice_sz()
		+ UI_Control::get_default_width( e_ui_control_type::button )
		+ slice_def->get_right_slice_sz();

	float_t panel_h =
		( UI_Control::get_default_height( e_ui_control_type::button ) * num_buttons )
		+ slice_def->get_top_slice_sz()
		+ ( get_ui_callback()->get_control_margin( e_ui_control_type::button ).y * ( num_buttons - 1 ) )
		+ slice_def->get_bottom_slice_sz()
		+ 12.f;

	g_ui->layout_init( Rect( 0.f, 0.f, panel_w, panel_h ) );

	g_ui->panel_control( H( "main_panel" ) )
		->center_control_on_screen()
		->adjust_layout_to_client_area()
		->done();

	{
		g_ui->caption_control()
			->set_text( "PAUSE MENU" )
			->done();

		g_ui->spacer_control()
			->done();

		if( g_ui->button_control( H( "button_resume" ) )
			->set_text( "Resume" )
			->done() )
		{
			g_engine->scene_mgr.pop();
		}

		if( g_base_game->flags.has_main_menu and !g_engine->scene_mgr.get_under()->flags.is_main_menu )
		{
			if( g_ui->button_control( H( "button_main_menu" ) )
				->set_text( "Main Menu" )
				->done() )
			{
				Timeline_Of_Nodes tl;
				tl.init( 1000 );

				tl.add_kf_pp_color_overlay(	false, 0.0f, 500, make_color( Color::black, 1.f ) );
				tl.add_kf_pp_pixelate(		false, 0.0f, 500, 32.f );
				tl.add_kf_scene_pop_under(	false, 0.475f );
				tl.add_kf_pp_pixelate(		false, 0.5f, 500, 0.f );
				tl.add_kf_pp_color_overlay(	false, 0.5f, 500, make_color( Color::black, 0.f ) );
				tl.add_kf_scene_pop_under(	false, 0.525f );

				g_engine->scene_mgr.do_transient_timeline( tl );
			}
		}

		if( g_ui->button_control( H( "button_exit" ) )
			->set_text( "Exit To Desktop" )
			->done() )
		{
			g_engine->is_running = false;
		}

		g_ui->divider_control()
			->done();

		if( g_ui->check_control( H( "check_fullscreen" ) )
			->set_text( "Full Screen?" )
			->done() )
		{
			g_engine->window.toggle_fullscreen();
		}
	}
}

}
