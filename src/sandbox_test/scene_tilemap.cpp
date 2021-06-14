
#include "app_header.h"

using namespace war;

scene_tilemap::scene_tilemap()
{
	flags.draws_completely_solid = true;
}

void scene_tilemap::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	ts_ultima = g_engine->find_asset<tile_set_asset>( "ts_ultima" );
	tm_overworld = g_engine->find_asset<tile_map_asset>( "tm_overworld" );
}

void scene_tilemap::draw()
{
	draw_tiled_background();
	scene::draw();

	{
		scoped_render_state;

		render::state->color = color::white;
		render::state->z += zdepth_nudge;
		render::draw_tile_map( ts_ultima, tm_overworld, vec2( 0.f, 0.f ) );
	}

	render::draw_world_axis();
}

void scene_tilemap::draw_ui()
{
	draw_title( "Tile Map" );
	scene::draw_ui();
}
