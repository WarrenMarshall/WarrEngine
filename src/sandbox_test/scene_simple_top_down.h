
using namespace war;

struct scene_simple_top_down : scene
{
	scene_simple_top_down();

	entity* player = nullptr;
	entity* hit_marker = nullptr;
	entity* world = nullptr;

	float ray_cast_length_hit = 0.0f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
