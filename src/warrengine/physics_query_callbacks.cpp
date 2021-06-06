
#include "master_pch.h"
#include "master_header.h"

namespace war
{

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

float raycast_closest::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	if( collision_mask > 0 )
	{
		auto ecp = ( (entity_component*)( fixture->GetBody()->GetUserData().pointer ) )->parent_entity->get_component<physics_component>();

		if( ( ecp->collision_mask & collision_mask ) == 0 )
		{
			return -1.f;
		}
	}

	hit_something = true;

	result.fraction = fraction;
	result.normal = vec2( normal.x, normal.y );
	result.pos = vec2( from_box2d( point.x ), from_box2d( point.y ) );

	return fraction;
}

// ----------------------------------------------------------------------------

float raycast_simple::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	if( collision_mask > 0 )
	{
		auto ecp = ( (entity_component*)( fixture->GetBody()->GetUserData().pointer ) )->parent_entity->get_component<physics_component>();

		if( ( ecp->collision_mask & collision_mask ) == 0 )
		{
			return -1.f;
		}
	}

	hit_something = true;

	result.fraction = fraction;
	result.normal = vec2( normal.x, normal.y );
	result.pos = vec2( from_box2d( point.x ), from_box2d( point.y ) );

	return 0.f;
}

// ----------------------------------------------------------------------------

float raycast_all::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction )
{
	if( collision_mask > 0 )
	{
		auto ecp = ( (entity_component*)( fixture->GetBody()->GetUserData().pointer ) )->parent_entity->get_component<physics_component>();

		if( ( ecp->collision_mask & collision_mask ) == 0 )
		{
			return -1.f;
		}
	}

	hit_something = true;

	raycast_hit hit;
	hit.fraction = fraction;
	hit.normal = vec2( normal.x, normal.y );
	hit.pos = vec2( from_box2d( point.x ), from_box2d( point.y ) );

	results.push_back( std::move( hit ) );

	return 1.f;
}

// ----------------------------------------------------------------------------

bool query_all::ReportFixture( b2Fixture* fixture )
{
	this->fixtures.push_back( fixture );
	return true;
}

bool query_first::ReportFixture( b2Fixture* fixture )
{
	this->fixture = fixture;
	return false;
}

}
