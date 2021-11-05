
using namespace war;

struct Scene_Entity_Picking final : Scene
{
	Scene_Entity_Picking();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void post_update() override;

	virtual bool on_input( const Input_Event* evt ) override;
};
