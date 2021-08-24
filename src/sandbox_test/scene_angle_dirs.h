
using namespace war;

struct scene_angle_dirs final : scene
{
	scene_angle_dirs();

	vec2 marker_pos = vec2::zero;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const input_event* evt ) override;
};
