
using namespace war;


struct Scene_Top_Down final : Scene
{
	struct
	{
		ui16 player = 0b00000001;
		ui16 geo	= 0b00000010;
	} coll_flags;

	Scene_Top_Down();

	Entity* player = nullptr;
	Entity* hit_marker = nullptr;
	Entity* world = nullptr;

	Timeline_Of_Nodes fx_hurt;

	f32 ray_cast_length_hit = 0.0f;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input( const Input_Event* evt ) override;

	virtual bool on_entity_and_sensor_touching( Entity* entity, Collision_Body_Component* sensor ) override;
};
