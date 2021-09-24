
using namespace war;

struct Scene_Spatial final : Scene
{
	Scene_Spatial();

	float_t glow_val = 0.f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
};
