
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void physics_contact_listener::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();

	physics_pending_collision pc;

	pc.entity_a = ( (entity_component*)( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (entity_component*)( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	assert( pc.entity_a->get_life_cycle()->is_alive() );
	assert( pc.entity_b->get_life_cycle()->is_alive() );

	g_engine->begin_contact_queue.emplace_back( std::move( pc ) );
}

void physics_contact_listener::EndContact( b2Contact* contact )
{
	physics_pending_collision pc;

	this->contact = contact;
	manifold = contact->GetManifold();

	pc.entity_a = ( (entity_component*)( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (entity_component*)( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	assert( pc.entity_a->get_life_cycle()->is_alive() );
	assert( pc.entity_b->get_life_cycle()->is_alive() );

	g_engine->end_contact_queue.emplace_back( pc );
}

vec2 physics_contact_listener::calc_hit_normal( b2Body* body_colliding )
{
	vec2 hit_normal = vec2::zero;

	if( manifold->type == 0 )	// circle
	{
		hit_normal = vec2( body_colliding->GetWorldCenter() ) - vec2( manifold->localPoint );
	}
	else
	{
		hit_normal = vec2( manifold->localNormal.x, manifold->localNormal.y );
	}

	return hit_normal.normalize();
}

}
