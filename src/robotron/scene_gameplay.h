
struct Scene_Gameplay final : Scene
{
	Player_Entity* player = nullptr;

	Scene_Gameplay();

	void move_player_by_delta( const Vec2& delta );

	virtual void pushed() override;
	virtual void popped() override;
	virtual bool on_input( const Input_Event* evt ) override;
	virtual bool on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, collision::Pending_Collision& coll ) override;
};
