
using namespace war;

struct scene_meshes final : scene
{
	Mesh_Component* mesh_player = nullptr;
	Mesh_Component* mesh_crate = nullptr;

	Tween mesh_rotator = {};
	Tween mesh_scaler = {};

	scene_meshes();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
