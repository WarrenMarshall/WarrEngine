
using namespace war;

struct scene_meshes : scene
{
	mesh_component* mesh_mario = nullptr;
	mesh_component* mesh_crate = nullptr;

	tween mesh_rotator = {};
	tween mesh_scaler = {};

	scene_meshes();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
