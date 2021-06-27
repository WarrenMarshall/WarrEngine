
using namespace war;

struct scene_simple_collision : scene
{
	scene_simple_collision();

	entity* mario = nullptr;
	entity* hit_marker = nullptr;
	entity* world_geo = nullptr;

	bool b_show_ray = false;
	vec2 ray_dir = {};

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
