
using namespace war;

struct Scene_Simple_Interact : Scene
{
	Scene_Simple_Interact();

	Entity* player = nullptr;
	Entity* hit_marker = nullptr;
	Entity* world_geo = nullptr;

	bool b_show_ray = false;
	Vec2 ray_dir = {};

	bool first_player = true;
	Entity* spawn_player();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
