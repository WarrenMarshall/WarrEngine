
#include "app_header.h"

using namespace war;

scene_tilemap::scene_tilemap()
{
	flags.blocks_further_drawing = true;
}

void scene_tilemap::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	ts_ultima = g_engine->find_asset<Tile_Set_Asset>( "ts_ultima" );
	tm_overworld = g_engine->find_asset<Tile_Map_Asset>( "tm_overworld" );
}

void scene_tilemap::draw()
{
	draw_tiled_background();
	scene::draw();

	{
		scoped_render_state;

		Render::state->color = Color::white;
		Render::state->z += zdepth_nudge;
		Render::draw_tile_map( ts_ultima, tm_overworld, Vec2( 0.f, 0.f ) );
	}

	Render::draw_world_axis();
}

void scene_tilemap::draw_ui()
{
	draw_title( "Tile Map" );
	scene::draw_ui();
}
