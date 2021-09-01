
using namespace war;

struct E_Sensor_Player : Entity
{
	virtual bool on_touched( simple_collision::Pending_Collision& coll ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Simple_Sensors : Scene
{
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
