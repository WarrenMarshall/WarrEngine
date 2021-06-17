
struct e_emoji_ball : entity
{
	virtual void on_box2d_collision_begin( box2d_physics::pending_collision& coll, entity* other ) override;
};
