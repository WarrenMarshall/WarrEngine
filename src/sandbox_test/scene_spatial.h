
using namespace war;

struct Scene_Spatial final : Scene
{
	struct
	{
		ui16 player = 0b0000001;
		ui16 skull = 0b0000010;
	} coll_flags;

	Entity* player_shape = nullptr;

	Scene_Spatial();

	void spawn_entity( Vec2 pos );
	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
