
#include "master_pch.h"
#include "master_header.h"

void w_physics_responder::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();
	contact_ids[ 0 ] = (const char*) contact->GetFixtureA()->GetUserData().pointer;
	contact_ids[ 1 ] = (const char*) contact->GetFixtureB()->GetUserData().pointer;
}

void w_physics_responder::EndContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();
	contact_ids[ 0 ] = (const char*) contact->GetFixtureA()->GetUserData().pointer;
	contact_ids[ 1 ] = (const char*) contact->GetFixtureB()->GetUserData().pointer;
}

void w_physics_responder::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
	this->contact = contact;
	manifold = contact->GetManifold();
}

void w_physics_responder::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	this->contact = contact;
}

bool w_physics_responder::contact_ids_match( const char* id_0, const char* id_1 )
{
	return(
		( contact_ids[ 0 ] == id_0 && contact_ids[ 1 ] == id_1 )
		|| ( contact_ids[ 0 ] == id_1 && contact_ids[ 1 ] == id_0 )
		);
}

// looks at the 2 fixtures involved in this contact and returns the requested one.
b2Fixture* w_physics_responder::find_fixture_from_contact_id( const char* id )
{
	if( contact_ids[ 0 ] == id )
	{
		return contact->GetFixtureA();
	}
	if( contact_ids[ 1 ] == id )
	{
		return contact->GetFixtureB();
	}

	// contact id not found
	assert( false );

	return nullptr;
}

b2Body* w_physics_responder::find_body_from_contact_id( const char* id )
{
	return find_fixture_from_contact_id( id )->GetBody();
}

w_entity_component* w_physics_responder::find_component_from_contact_id( const char* id )
{
	return (w_entity_component*) ( find_body_from_contact_id( id )->GetUserData().pointer );
}

w_entity* w_physics_responder::find_entity_from_contact_id( const char* id )
{
	return find_component_from_contact_id( id )->parent_entity;
}

w_vec2 w_physics_responder::calc_hit_normal( b2Body* body_colliding )
{
	w_vec2 hit_normal = w_vec2::zero;

	if( manifold->type == 0 )	// circle
	{
		hit_normal = w_vec2( body_colliding->GetWorldCenter() ) - w_vec2( manifold->localPoint );
	}
	else
	{
		hit_normal = w_vec2( manifold->localNormal.x, manifold->localNormal.y );
	}

	return hit_normal.normalize();
}

// ----------------------------------------------------------------------------

bool w_physics_query::trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask )
{
	w_raycast_simple callback;
	engine->box2d_world->RayCast( &callback, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );

	return callback.hit_something;
}

bool w_physics_query::trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_simple* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );

	return hit_result->hit_something;
}

bool w_physics_query::trace_closest( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_closest* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );
	return hit_result->hit_something;
}

bool w_physics_query::trace_all( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_all* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d().as_b2Vec2(), ( start + ( normal * dist ) ).to_b2d().as_b2Vec2() );
	return hit_result->hit_something;
}

bool w_physics_query::point_check_simple( w_vec2 pos, bitflags layer_mask )
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

bool w_physics_query::point_check_simple( w_vec2 pos, bitflags layer_mask, w_query_first* hit_result )
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

bool w_physics_query::point_check_all( w_vec2 pos, bitflags layer_mask, w_query_all* hit_result )
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
