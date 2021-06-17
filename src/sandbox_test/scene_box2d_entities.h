
struct e_emoji_ball : entity
{
	virtual void on_collision_begin( box2d_physics::pending_collision& coll, entity* other ) override;
};
