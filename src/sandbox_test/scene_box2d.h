
using namespace war;

struct scene_box2d final : scene
{
	scene_box2d();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	void spawn_ball_at( Vec2 world_pos );
	void spawn_box_at( Vec2 world_pos );
	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
