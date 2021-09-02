
#include "app_header.h"

using namespace war;

Scene_Palette::Scene_Palette()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Palette::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );
}

void Scene_Palette::draw()
{
	Render::draw_world_axis();
	draw_tiled_background();
	Scene::draw();
}

void Scene_Palette::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Palettes" );

	Rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_init( rc_panel );

	{
		scoped_render_state;

		if( g_ui->button_control()
			->set_text( "Ink" )
			->done() )
		{
			g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_ink" ) );
		}

		if( g_ui->button_control()
			->set_text( "Lumpy" )
			->done() )
		{
			g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_lumpy" ) );
		}

		if( g_ui->button_control()
			->set_text( "Azul 5" )
			->done() )
		{
			g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_azul5" ) );
		}

		if( g_ui->button_control()
			->set_text( "Poison" )
			->done() )
		{
			g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_poison" ) );
		}
	}
}
