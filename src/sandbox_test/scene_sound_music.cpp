
#include "app_header.h"

Scene_Sound_Music::Scene_Sound_Music()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Sound_Music::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Sound_Music::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Sound Board" );

	Rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_init( rc_panel );

	{
		scoped_render_state;

		if( g_ui->button_control()
			->set_text( "Coin" )
			->done() )
		{
			g_engine->find_asset<Sound_Asset>( "sfx_01" )->play();
		}

		if( g_ui->button_control()
			->set_text( "Jump" )
			->done() )
		{
			g_engine->find_asset<Sound_Asset>( "sfx_02" )->play();
		}

		if( g_ui->button_control()
			->set_text( "Doom" )
			->done() )
		{
			g_engine->find_asset<Sound_Asset>( "sfx_doom" )->play();
		}

		g_ui->divider_control()
			->done();

		{
			scoped_ui_layout( g_ui->layout_top().cut_top( 24.f ) );

			auto button_width = g_ui->layout_top().w / 5.f;

			g_ui->cut_left( button_width );

			if( g_ui->button_control()
				->set_text( "Play" )
				->cut_left( button_width )
				->done() )
			{
				g_engine->find_asset<Sound_Asset>( "music_01" )->play();
			}

			if( g_ui->button_control()
				->set_text( "Pause" )
				->cut_left( button_width )
				->done() )
			{
				g_engine->find_asset<Sound_Asset>( "music_01" )->pause();
			}

			if( g_ui->button_control()
				->set_text( "Stop" )
				->cut_left( button_width )
				->done() )
			{
				g_engine->find_asset<Sound_Asset>( "music_01" )->stop();
			}
		}
	}
}

void Scene_Sound_Music::popped()
{
	g_engine->find_asset<Sound_Asset>( "music_01" )->stop();

	Scene::popped();
}
