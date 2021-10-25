
namespace war::collision
{

// ----------------------------------------------------------------------------

struct Raycast_Hit final
{
	// how far along the ray did the hit occur?
	f32 dist = 999999.f;

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
	Collision_Body_Component* scc = nullptr;
};

// ----------------------------------------------------------------------------

struct Raycast_Callback
{
	virtual ~Raycast_Callback() = default;

	bool hit_something = false;
	i32 collision_mask = 0;

	virtual f32 report_component( const Entity* entity, const c2Ray& ray, Collision_Body_Component* scc, const c2Raycast& raycast ) = 0;
};

// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct Raycast_Closest final : Raycast_Callback
{
	Raycast_Hit result;

	virtual f32 report_component( const Entity* entity, const c2Ray& ray, Collision_Body_Component* scc, const c2Raycast& raycast ) override;
};

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct Raycast_Quick final : Raycast_Callback
{
	virtual f32 report_component( const Entity* entity, const c2Ray& ray, Collision_Body_Component* scc, const c2Raycast& raycast ) override;
};

// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct Raycast_All final : Raycast_Callback
{
	std::vector<Raycast_Hit> results;

	virtual f32 report_component( const Entity* entity, const c2Ray& ray, Collision_Body_Component* scc, const c2Raycast& raycast ) override;
};

}
