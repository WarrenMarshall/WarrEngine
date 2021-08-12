
namespace war::simple_collision
{

// ----------------------------------------------------------------------------

struct raycast_hit
{
	// how far along the ray did the hit occur?
	float dist = 999999.f;

	// the normal direction from the hit location. this is NOT the normal of the
	// ray that was cast, this is the normal of the impact.
	vec2 normal = vec2::zero;

	// the normal that the original ray was cast along.
	vec2 ray_normal = vec2::zero;

	// the world space position of the hit.
	vec2 pos = vec2::zero;

	// the entity that initiated the trace
	const entity* entity = nullptr;

	// the collision component that was hit by the ray. this belongs to the
	// entity that was hit, not the one doing the tracing.
	ec_simple_collision_body* scc = nullptr;
};

// ----------------------------------------------------------------------------

struct raycast_callback
{
	bool hit_something = false;
	int collision_mask = 0;

	virtual float report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast ) = 0;
};

// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct raycast_closest final : raycast_callback
{
	raycast_hit result;

	virtual float report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast ) override;
};

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct raycast_quick final : raycast_callback
{
	virtual float report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast ) override;
};

// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct raycast_all final : raycast_callback
{
	std::vector<raycast_hit> results;

	virtual float report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast ) override;
};

}
