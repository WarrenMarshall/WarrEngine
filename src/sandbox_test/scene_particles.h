
using namespace war;

struct scene_particles : scene
{
	scene_particles();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const input_event* evt ) override;
};