
namespace war::box2d
{

struct Raycast_Hit final
{
	// how far along the ray did the hit occur?
	f32 fraction = 0.f;

	// the normal direction resulting from the hit.
	Vec2 normal = Vec2::zero;

	// the world space position of the hit.
	Vec2 pos = Vec2::zero;
};

// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct Raycast_Closest final : b2RayCastCallback
{
	bool hit_something = false;
	i32 collision_mask = 0;
	Raycast_Hit result;

	virtual f32 ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, f32 fraction ) override;
};

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct Raycast_Simple final : b2RayCastCallback
{
	bool hit_something = false;
	i32 collision_mask = 0;
	Raycast_Hit result;

	virtual f32 ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, f32 fraction ) override;
};

// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct Raycast_All final : b2RayCastCallback
{
	bool hit_something = false;
	i32 collision_mask = 0;
	std::vector<Raycast_Hit> results;

	virtual f32 ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, f32 fraction ) override;
};

// ----------------------------------------------------------------------------
// stops after finding the first fixture that intersects with the AABB being queried

class Touching_First : public b2QueryCallback
{
public:
	b2Fixture* fixture;

	virtual bool ReportFixture( b2Fixture* fixture ) override;
};

// ----------------------------------------------------------------------------
// builds a list of all the fixtures that intersect with the AABB being queried

class touching_all : public b2QueryCallback
{
public:
	std::vector<b2Fixture*> fixtures;

	virtual bool ReportFixture( b2Fixture* fixture ) override;
};

}
