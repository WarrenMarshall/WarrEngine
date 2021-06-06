
#include "app_header.h"

using namespace war;

scene_palette::scene_palette()
{
	draws_completely_solid = true;
}

void scene_palette::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_palette::draw()
{
	render::draw_world_axis();
	draw_tiled_background();
	scene::draw();
}

void scene_palette::draw_ui()
{
	scene::draw_ui();
	draw_title( "Palettes" );

	rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_clear( rc_panel );

	{
		scoped_render_state;

		if( g_ui->button_control()
			->set_text( "Ink" )
			->end() )
		{
			g_engine->renderer.palette = *( g_engine->find_asset<palette_asset>( "pal_ink" ) );
		}

		if( g_ui->button_control()
			->set_text( "Lumpy" )
			->end() )
		{
			g_engine->renderer.palette = *( g_engine->find_asset<palette_asset>( "pal_lumpy" ) );
		}

		if( g_ui->button_control()
			->set_text( "Azul 5" )
			->end() )
		{
			g_engine->renderer.palette = *( g_engine->find_asset<palette_asset>( "pal_azul5" ) );
		}

		if( g_ui->button_control()
			->set_text( "Poison" )
			->end() )
		{
			g_engine->renderer.palette = *( g_engine->find_asset<palette_asset>( "pal_poison" ) );
		}
	}
}
