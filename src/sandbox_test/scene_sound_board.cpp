
#include "app_header.h"

scene_sound_board::scene_sound_board()
{
	flags.draws_completely_solid = true;
}

void scene_sound_board::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_sound_board::draw_ui()
{
	scene::draw_ui();
	draw_title( "Sound Board" );

	rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_init( rc_panel );

	{
		scoped_render_state;

		if( g_ui->button_control()
			->set_text( "Coin" )
			->end() )
		{
			g_engine->find_asset<sound_asset>( "sfx_01" )->play();
		}

		if( g_ui->button_control()
			->set_text( "Jump" )
			->end() )
		{
			g_engine->find_asset<sound_asset>( "sfx_02" )->play();
		}

		if( g_ui->button_control()
			->set_text( "Doom" )
			->end() )
		{
			g_engine->find_asset<sound_asset>( "sfx_doom" )->play();
		}

		g_ui->divider_control()
			->end();

		{
			scoped_ui_layout( g_ui->layout_top().cut_top( 24.f ) );

			auto button_width = g_ui->layout_top().w / 5.0f;

			g_ui->cut_left( button_width );

			if( g_ui->button_control()
				->set_text( "Play" )
				->cut_left( button_width )
				->end() )
			{
				g_engine->find_asset<sound_asset>( "music_01" )->play();
			}

			if( g_ui->button_control()
				->set_text( "Pause" )
				->cut_left( button_width )
				->end() )
			{
				g_engine->find_asset<sound_asset>( "music_01" )->pause();
			}

			if( g_ui->button_control()
				->set_text( "Stop" )
				->cut_left( button_width )
				->end() )
			{
				g_engine->find_asset<sound_asset>( "music_01" )->stop();
			}
		}
	}
}

void scene_sound_board::popped()
{
	g_engine->find_asset<sound_asset>( "music_01" )->stop();

	scene::popped();
}
