
using namespace war;

struct Scene_Simple_Platformer : Scene
{
	Entity* world = nullptr;
	Entity* player = nullptr;

	Timeline_Nodes fx_red_alert;

	Scene_Simple_Platformer();

	virtual void draw_ui() override;
	virtual void draw() override;
	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
