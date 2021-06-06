
using namespace war;

struct scene_tilemap : scene
{
	tile_set_asset* ts_ultima = nullptr;
	tile_map_asset* tm_overworld = nullptr;

	scene_tilemap();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
