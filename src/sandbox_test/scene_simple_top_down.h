
using namespace war;

struct Scene_Simple_Top_Down final : Scene
{
	struct
	{
		uint16_t player = 0b00000001;
		uint16_t geo	= 0b00000010;
	} coll_flags;

	Scene_Simple_Top_Down();

	Entity* player = nullptr;
	Entity* hit_marker = nullptr;
	Entity* world = nullptr;

	float_t ray_cast_length_hit = 0.0f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
