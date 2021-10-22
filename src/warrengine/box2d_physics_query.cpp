
#include "master_pch.h"
#include "master_header.h"

namespace war::box2d_physics
{

bool Query::trace_quick( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask )
{
	Raycast_Simple callback;
	callback.collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( &callback, start.to_box2d().to_b2Vec2(), ( start + ( normal * dist ) ).to_box2d().to_b2Vec2() );

	return callback.hit_something;
}

bool Query::trace_quick( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask, Raycast_Simple* hit_result )
{
	hit_result->collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( hit_result, start.to_box2d().to_b2Vec2(), ( start + ( normal * dist ) ).to_box2d().to_b2Vec2() );

	return hit_result->hit_something;
}

bool Query::trace_closest( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask, Raycast_Closest* hit_result )
{
	hit_result->collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( hit_result, start.to_box2d().to_b2Vec2(), ( start + ( normal * dist ) ).to_box2d().to_b2Vec2() );
	return hit_result->hit_something;
}

bool Query::trace_all( const Vec2& start, const Vec2& normal, f32 dist, i32 collision_mask, Raycast_All* hit_result )
{
	hit_result->collision_mask = collision_mask;
	g_engine->box2d.world->RayCast( hit_result, start.to_box2d().to_b2Vec2(), ( start + ( normal * dist ) ).to_box2d().to_b2Vec2() );
	return hit_result->hit_something;
}

bool Query::point_check_simple( const Vec2& pos )
{
	Touching_First hit_result;
	b2Vec2 bpos = pos.to_box2d().to_b2Vec2();

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

bool Query::point_check_simple( const Vec2& pos, Touching_First* hit_result )
{
	b2Vec2 bpos = pos.to_box2d().to_b2Vec2();

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

bool Query::point_check_all( const Vec2& pos, touching_all* hit_result )
{
	b2Vec2 bpos = pos.to_box2d().to_b2Vec2();

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

}
