
using namespace war;

struct scene_angle_dirs final : scene
{
	scene_angle_dirs();

	Vec2 marker_pos = Vec2::zero;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
};
