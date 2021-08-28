
using namespace war;

struct scene_simple_top_down final : scene
{
	scene_simple_top_down();

	Entity* player = nullptr;
	Entity* hit_marker = nullptr;
	Entity* world = nullptr;

	float ray_cast_length_hit = 0.0f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
