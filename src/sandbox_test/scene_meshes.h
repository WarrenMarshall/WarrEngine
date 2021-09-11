
using namespace war;

struct Scene_Meshes final : Scene
{
	Mesh_Component* mesh_player = nullptr;
	Mesh_Component* mesh_crate = nullptr;

	Tween mesh_rotator = {};
	Tween mesh_scaler = {};

	Scene_Meshes();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
