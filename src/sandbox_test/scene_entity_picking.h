
using namespace war;

struct scene_entity_picking final : scene
{
	scene_entity_picking();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void post_update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
