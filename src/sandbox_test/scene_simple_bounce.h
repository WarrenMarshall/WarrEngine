
using namespace war;

struct Scene_Simple_Bounce : Scene
{
	Scene_Simple_Bounce();

	// debug
	bool first_time = true;
	// debug

	Entity* player = nullptr;
	Entity* world_geo = nullptr;

	Entity* spawn_player();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void post_update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
