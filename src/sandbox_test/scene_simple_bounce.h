
using namespace war;

struct scene_simple_bounce final : scene
{
	scene_simple_bounce();

	// debug
	bool first_time = true;
	// debug

	entity* player = nullptr;
	entity* world_geo = nullptr;

	entity* spawn_player();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void post_update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
