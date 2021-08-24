
namespace war::box2d_physics
{

// called by box2d when collisions begin or end between bodies

struct contact_listener : b2ContactListener
{
	virtual ~contact_listener() = default;

	b2Contact* contact = nullptr;
	b2Manifold* manifold = nullptr;

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;

	[[nodiscard]] vec2 calc_hit_normal( b2Body* body_colliding );
};

}
