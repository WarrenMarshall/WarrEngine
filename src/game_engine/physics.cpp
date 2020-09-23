
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_contact_listener::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	sensor_ids[ 0 ] = (e_sensor_id) contact->GetFixtureA()->GetUserData().pointer;
	sensor_ids[ 1 ] = (e_sensor_id) contact->GetFixtureB()->GetUserData().pointer;
}

void w_contact_listener::EndContact( b2Contact* contact )
{
	this->contact = contact;
	sensor_ids[ 0 ] = (e_sensor_id) contact->GetFixtureA()->GetUserData().pointer;
	sensor_ids[ 1 ] = (e_sensor_id) contact->GetFixtureB()->GetUserData().pointer;
}

void w_contact_listener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
}

void w_contact_listener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
}

bool w_contact_listener::sensor_ids_match( const std::array<e_sensor_id, 2>& ids_to_check )
{
	return(
		( sensor_ids[ 0 ] == ids_to_check[ 0 ] && sensor_ids[ 1 ] == ids_to_check[ 1 ] )
		|| ( sensor_ids[ 0 ] == ids_to_check[ 1 ] && sensor_ids[ 1 ] == ids_to_check[ 0 ] )
		);
}


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

bool w_physics::point_check_simple( w_vec2 pos, e_collision_layer layer_mask )
{
	w_query_first hit_result;
	b2Vec2 bpos = pos.to_b2d();

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
