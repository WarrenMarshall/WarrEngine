
using namespace war;

struct scene_simple_collision : scene
{
	scene_simple_collision();

	entity* mario = nullptr;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
