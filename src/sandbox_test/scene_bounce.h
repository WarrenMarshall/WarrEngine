
using namespace war;

struct Scene_Bounce final : Scene
{
	struct
	{
		ui16 ball	= 0b00000001;
		ui16 shape	= 0b00000010;
		ui16 geo	= 0b00000100;
	} coll_flags;

	Scene_Bounce();

	Entity* spawn_shape();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void post_update() override;

	virtual bool on_input( const Input_Event* evt ) override;
};
