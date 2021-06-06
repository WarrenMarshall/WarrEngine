
using namespace war;

struct scene_box2d : scene
{
	scene_box2d();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	void spawn_ball_at( vec2 world_pos );
	void spawn_box_at( vec2 world_pos );
	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
