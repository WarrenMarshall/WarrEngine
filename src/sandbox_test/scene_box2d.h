
using namespace war;

struct Scene_Box2D : Scene
{
	Scene_Box2D();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	void spawn_ball_at( Vec2 world_pos );
	void spawn_box_at( Vec2 world_pos );
	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
