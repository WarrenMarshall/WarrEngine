
#include "master_pch.h"
#include "master_header.h"

namespace war::simple_collision
{

bool query::trace_simple( const entity* entity, const vec2& start, const vec2& normal, float dist, int collision_mask )
{
	raycast_quick callback;
	callback.collision_mask = collision_mask;
	g_engine->simple_collision.world->ray_cast( &callback, entity, start, ( start + ( normal * dist ) ) );

	return callback.hit_something;
}

/*
bool query::trace_simple( const vec2& start, const vec2& normal, float dist, int collision_mask, raycast_simple* hit_result )
{
	hit_result->collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( hit_result, start, ( start + ( normal * dist ) ) );

	return hit_result->hit_something;
}

bool query::trace_closest( const vec2& start, const vec2& normal, float dist, int collision_mask, raycast_closest* hit_result )
{
	hit_result->collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( hit_result, start, ( start + ( normal * dist ) ) );
	return hit_result->hit_something;
}

bool query::trace_all( const vec2& start, const vec2& normal, float dist, int collision_mask, raycast_all* hit_result )
{
	hit_result->collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( hit_result, start, ( start + ( normal * dist ) ) );
	return hit_result->hit_something;
}
*/

/*
bool query::point_check_simple( const vec2& pos )
{
	touching_first hit_result;
	b2Vec2 bpos = pos;

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	g_engine->box2d.world->QueryAABB( &hit_result, aabb );

	if( !hit_result.fixture->TestPoint( bpos ) )
	{
		return false;
	}

	return true;
}

bool query::point_check_simple( const vec2& pos, touching_first* hit_result )
{
	b2Vec2 bpos = pos;

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	g_engine->box2d.world->QueryAABB( hit_result, aabb );

	if( !hit_result->fixture->TestPoint( bpos ) )
	{
		hit_result->fixture = nullptr;
		return false;
	}

	return true;
}

bool query::point_check_all( const vec2& pos, touching_all* hit_result )
{
	b2Vec2 bpos = pos;

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	g_engine->box2d.world->QueryAABB( hit_result, aabb );

	std::vector<b2Fixture*> fixtures_hit;

	for( auto f : hit_result->fixtures )
	{
		if( f->TestPoint( bpos ) )
		{
			fixtures_hit.push_back( f );
		}
	}

	hit_result->fixtures = fixtures_hit;

	return !hit_result->fixtures.empty();
}
*/

}
