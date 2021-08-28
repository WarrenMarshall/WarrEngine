
using namespace war;

struct scene_tilemap final : scene
{
	Tile_Set_Asset* ts_ultima = nullptr;
	Tile_Map_Asset* tm_overworld = nullptr;

	scene_tilemap();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
