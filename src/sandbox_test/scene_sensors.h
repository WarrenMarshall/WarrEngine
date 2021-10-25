
using namespace war;

struct Scene_Sensors final : Scene
{
	struct
	{
		ui16 player = 0b0000001;
		ui16 world	= 0b0000010;
		ui16 sensor = 0b0000100;
	} coll_flags;

	Scene_Sensors();

	Entity* player = nullptr;
	Entity* world_geo = nullptr;

	void spawn_sensor();
	virtual void pushed() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;

	virtual bool on_entity_and_sensor_touching_begin( Entity* entity, Collision_Body_Component* sensor ) override;
	virtual bool on_entity_and_sensor_touching_end( Entity* entity, Collision_Body_Component* sensor ) override;
};
