
using namespace war;

struct Scene_Simple_Bounce final : Scene
{
	struct
	{
		uint16_t ball	= 0b00000001;
		uint16_t shape	= 0b00000010;
		uint16_t geo	= 0b00000100;
	} coll_flags;

	Scene_Simple_Bounce();

	Entity* spawn_shape();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void post_update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
