
#include "master_pch.h"
#include "master_header.h"

bool w_physics_query::trace_simple( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask )
{
	w_raycast_simple callback;
	callback.layer_mask = layer_mask;
	engine->box2d_world->RayCast( &callback, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );

	return callback.hit_something;
}

bool w_physics_query::trace_simple( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_simple* hit_result )
{
	hit_result->layer_mask = layer_mask;
	engine->box2d_world->RayCast( hit_result, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );

	return hit_result->hit_something;
}

bool w_physics_query::trace_closest( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_closest* hit_result )
{
	hit_result->layer_mask = layer_mask;
	engine->box2d_world->RayCast( hit_result, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );
	return hit_result->hit_something;
}

bool w_physics_query::trace_all( const w_vec2& start, const w_vec2& normal, float dist, int layer_mask, w_raycast_all* hit_result )
{
	hit_result->layer_mask = layer_mask;
	engine->box2d_world->RayCast( hit_result, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );
	return hit_result->hit_something;
}

bool w_physics_query::point_check_simple( const w_vec2& pos )
{
	w_query_first hit_result;
	b2Vec2 bpos = pos.to_b2d().as_b2Vec2();

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	engine->box2d_world->QueryAABB( &hit_result, aabb );

	if( !hit_result.fixture->TestPoint( bpos ) )
	{
		return false;
	}

	return true;
}

bool w_physics_query::point_check_simple( const w_vec2& pos, w_query_first* hit_result )
{
	b2Vec2 bpos = pos.to_b2d().as_b2Vec2();

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	engine->box2d_world->QueryAABB( hit_result, aabb );

	if( !hit_result->fixture->TestPoint( bpos ) )
	{
		hit_result->fixture = nullptr;
		return false;
	}

	return true;
}

bool w_physics_query::point_check_all( const w_vec2& pos, w_query_all* hit_result )
{
	b2Vec2 bpos = pos.to_b2d().as_b2Vec2();

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	engine->box2d_world->QueryAABB( hit_result, aabb );

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
