
#include "app_header.h"

using namespace war;

Scene_TileMap::Scene_TileMap()
{
	flags.blocks_further_drawing = true;
}

void Scene_TileMap::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	ts_ultima = g_engine->find_asset<Tile_Set_Asset>( "ts_ultima" );
	tm_overworld = g_engine->find_asset<Tile_Map_Asset>( "tm_overworld" );
}

void Scene_TileMap::draw()
{
	draw_tiled_background();
	Scene::draw();

	{
		scoped_render_state;

		Render::state->color = Color::white;
		Render::state->z += zdepth_nudge;
		Render::draw_tile_map( ts_ultima, tm_overworld, Vec2( 0.f, 0.f ) );
	}

	Render::draw_world_axis();
}

void Scene_TileMap::draw_ui()
{
	draw_title( "Tile Map" );
	Scene::draw_ui();
}
