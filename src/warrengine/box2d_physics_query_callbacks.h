
namespace war::box2d_physics
{

struct Raycast_Hit
{
	// how far along the ray did the hit occur?
	float fraction = 0.f;

	// the normal direction resulting from the hit.
	Vec2 normal = Vec2::zero;

	// the world space position of the hit.
	Vec2 pos = Vec2::zero;
};

// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct Raycast_Closest : b2RayCastCallback
{
	bool hit_something = false;
	int collision_mask = 0;
	Raycast_Hit result;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction ) override;
};

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct Raycast_Simple : b2RayCastCallback
{
	bool hit_something = false;
	int collision_mask = 0;
	Raycast_Hit result;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction ) override;
};

// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct Raycast_All : b2RayCastCallback
{
	bool hit_something = false;
	int collision_mask = 0;
	std::vector<Raycast_Hit> results;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float fraction ) override;
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
