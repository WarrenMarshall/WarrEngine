
using namespace war;

struct scene_simple_bounce final : scene
{
	scene_simple_bounce();

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
