
using namespace war;

struct Scene_Angle_Dirs final : Scene
{
	Scene_Angle_Dirs();

	Vec2 marker_pos = Vec2::zero;
	Vec2 marker_pos2_start = Vec2::zero;
	Vec2 marker_pos2 = Vec2::zero;

	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
