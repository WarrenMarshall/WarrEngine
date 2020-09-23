
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

bool w_physics::trace_simple( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask )
{
	w_raycast_simple callback;
	engine->box2d_world->RayCast( &callback, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );

	return callback.hit_something;
}

bool w_physics::trace_simple( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_simple* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );

	return hit_result->hit_something;
}

bool w_physics::trace_closest( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_closest* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );
	return hit_result->hit_something;
}

bool w_physics::trace_all( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_all* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );
	return hit_result->hit_something;
}

bool w_physics::point_check_simple( w_vec2 pos, e_collision_layer layer_mask, w_query_first* hit_result )
{
	b2Vec2 bpos = pos.to_b2d();

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

bool w_physics::point_check_all( w_vec2 pos, e_collision_layer layer_mask, w_query_all* hit_result )
{
	b2Vec2 bpos = pos.to_b2d();

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

	return ( hit_result->fixtures.size() > 0);
}
