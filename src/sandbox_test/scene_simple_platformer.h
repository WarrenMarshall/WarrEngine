
using namespace war;

struct scene_simple_platformer final : scene
{
	Entity* world = nullptr;
	Entity* player = nullptr;

	Timeline_Nodes fx_red_alert;

	scene_simple_platformer();

	virtual void draw_ui() override;
	virtual void draw() override;
	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
