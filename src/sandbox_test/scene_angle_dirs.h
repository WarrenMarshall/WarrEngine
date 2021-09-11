
using namespace war;

struct Scene_Angle_Dirs : Scene
{
	Scene_Angle_Dirs();

	Vec2 marker_pos = Vec2::zero;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
};
