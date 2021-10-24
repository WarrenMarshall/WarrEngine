
struct Scene_Gameplay final : Scene
{
	E_Player* player = nullptr;

	Scene_Gameplay();

	virtual void pushed() override;
	virtual void draw() override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
	virtual bool on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, simple_collision::Pending_Collision& coll ) override;
};
