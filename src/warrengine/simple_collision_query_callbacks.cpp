
#include "master_pch.h"
#include "master_header.h"

namespace war::simple_collision
{

// #comment - this comment needs to be updated once all the simple ray casting is working
// ----------------------------------------------------------------------------
// these raycast_callback functions filter hit results from a raycast.
//
// in short, it depends on what you want to do:
//
// To find only the closest intersection :
//		- return the fraction value from the callback
//		- use the most recent intersection as the result
// To find all intersections along the ray :
//		- return 1 from the callback
//		- store the intersections in a list
// To simply find if the ray hits anything :
//		- if you get a callback, something was hit( but it may not be the closest )
//		- return 0 from the callback for efficiency
//
// returning -1 from any ReportFixture function means you want to ignore that fixture
// ----------------------------------------------------------------------------

float_t Raycast_Closest::report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast )
{
	hit_something = true;

	auto start_of_trace = Vec2( ray.p.x, ray.p.y ).from_simple();
	auto trace_normal = Vec2( ray.d.x, ray.d.y );

	if( raycast.t < result.dist )
	{
		Raycast_Hit hit;
		hit.normal.x = raycast.n.x;
		hit.normal.y = raycast.n.y;
		hit.ray_normal.x = ray.d.x;
		hit.ray_normal.y = ray.d.y;
		hit.dist = raycast.t;
		hit.pos = start_of_trace + ( trace_normal * hit.dist );
		hit.entity = entity;
		hit.scc = scc;

		result = hit;
	}

	return 1.f;
}

// ----------------------------------------------------------------------------

float_t Raycast_Quick::report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast )
{
	hit_something = true;

	// a raycast_quick always returns zero because we only care if we DID collide, not any of the specifics
	return 0.f;
}

// ----------------------------------------------------------------------------

float_t Raycast_All::report_component( const Entity* entity, const c2Ray& ray, Simple_Collision_Body* scc, const c2Raycast& raycast )
{
	hit_something = true;

	auto start_of_trace = Vec2( ray.p.x, ray.p.y ).from_simple();;
	auto trace_normal = Vec2( ray.d.x, ray.d.y );

	Raycast_Hit hit;
	hit.normal.x = raycast.n.x;
	hit.normal.y = raycast.n.y;
	hit.ray_normal.x = ray.d.x;
	hit.ray_normal.y = ray.d.y;
	hit.dist = raycast.t;
	hit.pos = start_of_trace + ( trace_normal * hit.dist );
	hit.entity = entity;
	hit.scc = scc;

	// convert back to normal space

	results.push_back( hit );

	return 1.f;
}

}
