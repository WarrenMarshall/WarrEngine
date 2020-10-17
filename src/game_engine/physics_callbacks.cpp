
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
// returning -1 from any ReportFixture function means you want to ignore that fixture
// ----------------------------------------------------------------------------

float w_raycast_closest::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	hit_something = true;

	result.fraction = fraction;
	result.normal = w_vec2( normal.x, normal.y );
	result.point = w_vec2( from_b2d( point.x ), from_b2d( point.y ) );

	return fraction;
}

// ----------------------------------------------------------------------------

float w_raycast_simple::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	hit_something = true;

	result.fraction = fraction;
	result.normal = w_vec2( normal.x, normal.y );
	result.point = w_vec2( from_b2d( point.x ), from_b2d( point.y ) );

	return 0.0f;
}

// ----------------------------------------------------------------------------

float w_raycast_all::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	hit_something = true;

	w_raycast_hit hit;
	hit.fraction = fraction;
	hit.normal = w_vec2( normal.x, normal.y );
	hit.point = w_vec2( from_b2d( point.x ), from_b2d( point.y ) );

	results.push_back( std::move( hit ) );

	return 1.0f;
}

// ----------------------------------------------------------------------------

bool w_query_all::ReportFixture( b2Fixture* fixture )
{
	this->fixtures.push_back( fixture );
	return true;
}

bool w_query_first::ReportFixture( b2Fixture* fixture )
{
	this->fixture = fixture;
	return false;
}
