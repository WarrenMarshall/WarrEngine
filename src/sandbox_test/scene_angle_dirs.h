
using namespace war;

struct scene_angle_dirs : scene
{
	scene_angle_dirs();

	float glow_val = 0.0f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const input_event* evt ) override;
};
