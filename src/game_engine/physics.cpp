
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------
// these raycast_callback functions filter hit results from a Box2D raycast.
//
// details on how this works : http://www.iforce2d.net/b2dtut/world-querying
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
// if hit_point == std::nullopt, then the ray didn't hit anything at all.
// ----------------------------------------------------------------------------

float w_raycast_callback_closest::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	hit_point = point;

	return fraction;
}

// ----------------------------------------------------------------------------
