#pragma once

// ----------------------------------------------------------------------------

struct w_raycast_hit
{
	float fraction = 0.0f;
	w_vec2 normal = w_vec2::zero;
	w_vec2 point = w_vec2::zero;
};

// ----------------------------------------------------------------------------
// finds the closest hit along the ray.
//
// "what's the closest hit along this ray?"

struct w_raycast_closest : b2RayCastCallback
{
	bool hit_something = false;
	w_raycast_hit hit;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
								 const b2Vec2& normal, float fraction ) override;
};

// ----------------------------------------------------------------------------
// finds the first hit along the ray, which may or may not be the closest.
// this is the fastest version of a raycast - basically a boolean test.
//
// "is there anything to hit along this ray?"

struct w_raycast_simple : b2RayCastCallback
{
	bool hit_something = false;
	w_raycast_hit hit;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
								 const b2Vec2& normal, float fraction ) override;
};

// ----------------------------------------------------------------------------
// generates a list of all the hit points along the ray.
//
// "what are all the hits along this ray?"

struct w_raycast_all : b2RayCastCallback
{
	bool hit_something = false;
	std::vector<w_raycast_hit> hits;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
								 const b2Vec2& normal, float fraction ) override;
};

// ----------------------------------------------------------------------------
// builds a list of all the fixtures that intersect with the AABB being queried

class w_query_all : public b2QueryCallback
{
public:
	std::vector<b2Fixture*> fixtures;

	bool ReportFixture( b2Fixture* fixture );
};

// ----------------------------------------------------------------------------
// stops after finding the first fixture that intersects with the AABB being queried

class w_query_first : public b2QueryCallback
{
public:
	b2Fixture* fixture;

	bool ReportFixture( b2Fixture* fixture );
};
