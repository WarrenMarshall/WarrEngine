
using namespace war;

struct scene_gameplay : scene
{
	entity* player = nullptr;

	scene_gameplay();

	virtual void draw_ui() override;
	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
};
