
using namespace war;

struct scene_simple_collision : scene
{
	scene_simple_collision();

	entity* player = nullptr;
	entity* world_geo = nullptr;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
	void spawn_ball_at( vec2 world_pos );
	void spawn_box_at( vec2 world_pos );
};
