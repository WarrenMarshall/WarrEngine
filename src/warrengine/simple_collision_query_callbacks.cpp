
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

float raycast_closest::report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast )
{
	hit_something = true;

	auto start_of_trace = vec2( ray.p.x, ray.p.y ).from_simple();
	auto trace_normal = vec2( ray.d.x, ray.d.y );

	if( raycast.t < result.dist )
	{
		raycast_hit hit;
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

	// a raycast_all always returns 1 because we want a complete list of everything that got hit
	return 1.f;
}

// ----------------------------------------------------------------------------

float raycast_quick::report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast )
{
	hit_something = true;

	// a raycast_simple always returns zero because we only care if we DID collide, not any of the specifics
	return 0.f;
}

// ----------------------------------------------------------------------------

float raycast_all::report_component( const entity* entity, const c2Ray& ray, ec_simple_collision_body* scc, const c2Raycast& raycast )
{
	hit_something = true;

	auto start_of_trace = vec2( ray.p.x, ray.p.y ).from_simple();;
	auto trace_normal = vec2( ray.d.x, ray.d.y );

	raycast_hit hit;
	hit.normal.x = raycast.n.x;
	hit.normal.y = raycast.n.y;
	hit.ray_normal.x = ray.d.x;
	hit.ray_normal.y = ray.d.y;
	hit.dist = raycast.t;
	hit.pos = start_of_trace + ( trace_normal * hit.dist );
	hit.entity = entity;
	hit.scc = scc;

	// convert back to normal space

	//hit.pos = hit.pos.from_simple();
	results.push_back( hit );

	// a raycast_all always returns 1 because we want a complete list of everything that got hit
	return 1.f;
}

// ----------------------------------------------------------------------------

/*
bool touching_all::report_component( simple_collision_component* component )
{
	components.push_back( component );
	return true;
}

bool touching_first::report_component( simple_collision_component* component )
{
	this->component = component;
	return false;
}
*/

}
