
using namespace war;

struct E_Sensor_Player : Entity
{
	virtual bool on_touching_begin( Simple_Collision_Body* sensor ) override;
	virtual bool on_touching( Simple_Collision_Body* sensor ) override;
	virtual bool on_touching_end( Simple_Collision_Body* sensor ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Simple_Sensors : Scene
{
	struct
	{
		uint16_t player = 0b0000001;
		uint16_t world	= 0b0000010;
		uint16_t sensor = 0b0000100;
	} coll_flags;

	Scene_Simple_Sensors();

	Entity* player = nullptr;
	Entity* world_geo = nullptr;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
