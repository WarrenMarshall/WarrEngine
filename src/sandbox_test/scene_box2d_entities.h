
struct E_Emoji_Ball final : Entity
{
	virtual void on_box2d_collision_begin( box2d_physics::Pending_Collision& coll, Entity* other ) override;
};
