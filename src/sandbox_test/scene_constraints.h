
using namespace war;

struct E_Sensor_Player final : Entity
{
	i32 touch_count = 0;

	virtual bool on_touching_begin( Collision_Body* sensor ) override;
	virtual bool on_touching_end( Collision_Body* sensor ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Constraints final : Scene
{
	struct
	{
		ui16 player = 0b0000001;
		ui16 world	= 0b0000010;
		ui16 sensor = 0b0000100;
	} coll_flags;

	Scene_Constraints();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
