
struct Scene_Gameplay final : Scene
{
	Player_Entity* player = nullptr;

	Scene_Gameplay();

	virtual void pushed() override;
	void spawn_random_drone();
	virtual void popped() override;
	virtual void draw() override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
	virtual bool on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, collision::Pending_Collision& coll ) override;
};
