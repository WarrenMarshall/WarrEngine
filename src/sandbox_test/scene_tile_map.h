
using namespace war;

struct Scene_Tile_Map : Scene
{
	Tile_Set_Asset* ts_ultima = nullptr;
	Tile_Map_Asset* tm_overworld = nullptr;

	Scene_Tile_Map();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
