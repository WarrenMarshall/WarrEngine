
using namespace war;

struct Scene_Particles final : Scene
{
	Scene_Particles();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
};
