
using namespace war;

struct scene_simple_minigolf : scene
{
	scene_simple_minigolf();

	entity* player = nullptr;
	entity* world_geo = nullptr;

	bool b_is_aiming = false;
	vec2 aim_dir = {};
	float aim_force = 0.f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
