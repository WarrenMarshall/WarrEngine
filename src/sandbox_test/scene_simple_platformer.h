
using namespace war;

struct scene_simple_platformer : scene
{
	entity* world = nullptr;
	entity* player = nullptr;

	scene_simple_platformer();

	virtual void draw_ui() override;
	virtual void draw() override;
	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_motion( const input_event* evt ) override;
	virtual bool on_input_pressed( const input_event* evt ) override;
};
