
using namespace war;

struct E_Sensor_Player final : Entity
{
	int32_t touch_count = 0;

	virtual bool on_touching_begin( Simple_Collision_Body* sensor ) override;
	virtual bool on_touching_end( Simple_Collision_Body* sensor ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Simple_Sensors final : Scene
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

	void spawn_sensor( Entity* entity );
	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
