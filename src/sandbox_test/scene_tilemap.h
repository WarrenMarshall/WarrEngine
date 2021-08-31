
using namespace war;

struct Scene_TileMap : Scene
{
	Tile_Set_Asset* ts_ultima = nullptr;
	Tile_Map_Asset* tm_overworld = nullptr;

	Scene_TileMap();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
