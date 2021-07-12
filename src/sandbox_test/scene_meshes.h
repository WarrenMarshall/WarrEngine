
using namespace war;

struct scene_meshes : scene
{
	ec_mesh* mesh_mario = nullptr;
	ec_mesh* mesh_crate = nullptr;

	tween mesh_rotator = {};
	tween mesh_scaler = {};

	scene_meshes();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
