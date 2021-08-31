
using namespace war;

struct Scene_Simple_Collision : Scene
{
	Scene_Simple_Collision();

	Entity* player = nullptr;
	Entity* world_geo = nullptr;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
	void spawn_ball_at( Vec2 world_pos );
	void spawn_box_at( Vec2 world_pos );
};
