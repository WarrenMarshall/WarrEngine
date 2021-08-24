
using namespace war;

struct scene_simple_interact final : scene
{
	scene_simple_interact();

	entity* player = nullptr;
	entity* hit_marker = nullptr;
	entity* world_geo = nullptr;

	bool b_show_ray = false;
	vec2 ray_dir = {};

	bool first_player = true;
	entity* spawn_player();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
