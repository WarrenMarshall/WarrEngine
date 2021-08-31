
namespace war::simple_collision
{

// ----------------------------------------------------------------------------

struct Raycast_Hit
{
	// how far along the ray did the hit occur?
	float dist = 999999.f;

	// the normal direction from the hit location. this is NOT the normal of the
	// ray that was cast, this is the normal of the impact.
	Vec2 normal = Vec2::zero;

	// the normal that the original ray was cast along.
	Vec2 ray_normal = Vec2::zero;

	// the world space position of the hit.
	Vec2 pos = Vec2::zero;

	// the entity that initiated the trace
	const Entity* entity = nullptr;

	// the collision component that was hit by the ray. this belongs to the
	// entity that was hit, not the one doing the tracing.
	Simple_Collision_Body* scc = nullptr;
};

// ----------------------------------------------------------------------------

struct Raycast_Callback
{
	virtual ~Raycast_Callback() = default;

	bool hit_something = false;
	int collision_mask = 0;

	virtual float report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast ) = 0;
};

// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct Raycast_Closest : Raycast_Callback
{
	Raycast_Hit result;

	virtual float report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast ) override;
};

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct Raycast_Quick : Raycast_Callback
{
	virtual float report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast ) override;
};

// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct Raycast_All : Raycast_Callback
{
	std::vector<Raycast_Hit> results;

	virtual float report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast ) override;
};

}
