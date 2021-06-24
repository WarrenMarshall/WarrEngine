
namespace war::simple_collision
{

struct raycast_hit
{
	// how far along the ray did the hit occur?
	float fraction = 0.f;

	// the normal direction resulting from the hit.
	vec2 normal = vec2::zero;

	// the world space position of the hit.
	vec2 pos = vec2::zero;
};

// ----------------------------------------------------------------------------

struct raycast_callback
{
	bool hit_something = false;
	int collision_mask = 0;

	virtual float report_component( simple_collision_component* scc, const vec2& point, const vec2& normal, float fraction ) = 0;
};

/*
// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct raycast_closest final : raycast_callback
{
	raycast_hit result;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction ) override;
};
*/

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct raycast_simple final : raycast_callback
{
	raycast_hit result;

	virtual float report_component( simple_collision_component* scc, const vec2& point, const vec2& normal, float fraction ) override;
};

/*
// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct raycast_all final : raycast_callback
{
	bool hit_something = false;
	int collision_mask = 0;
	std::vector<raycast_hit> results;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction ) override;
};
*/

/*
// ----------------------------------------------------------------------------

struct simple_collision_query_callback
{
	virtual ~simple_collision_query_callback() {}

	virtual bool report_component( simple_collision_component* component ) = 0;
};

// ----------------------------------------------------------------------------
// builds a list of all the fixtures that intersect with the AABB being queried

struct touching_all final : simple_collision_query_callback
{
	std::vector<simple_collision_component*> components;

	virtual bool report_component( simple_collision_component* component ) override;
};

// ----------------------------------------------------------------------------
// stops after finding the first fixture that intersects with the AABB being queried

struct touching_first final : simple_collision_query_callback
{
	simple_collision_component* component;

	virtual bool report_component( simple_collision_component* component ) override;
};
*/

}
