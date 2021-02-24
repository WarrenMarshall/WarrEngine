
#include "master_pch.h"
#include "master_header.h"

void w_physics_contact_listener::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();

	w_physics_pending_collision pc;

	pc.entity_a = ( (w_entity_component*) ( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (w_entity_component*) ( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	engine->begin_contact_queue.emplace_back( std::move( pc ) );
}

void w_physics_contact_listener::EndContact( b2Contact* contact )
{
	w_physics_pending_collision pc;
	pc.fixture_b = contact->GetFixtureB();

	this->contact = contact;
	manifold = contact->GetManifold();

	pc.entity_a = ( (w_entity_component*) ( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (w_entity_component*) ( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.fixture_a = contact->GetFixtureA();
	pc.manifold = *contact->GetManifold();

	engine->end_contact_queue.emplace_back( pc );
}

w_vec2 w_physics_contact_listener::calc_hit_normal( b2Body* body_colliding )
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
