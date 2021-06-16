
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

	// either entity is dying, don't add this contact to the list.
	if( !pc.entity_a->get_life_cycle()->is_alive() or !pc.entity_b->get_life_cycle()->is_alive() )
	{
		return;
	}

	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	g_engine->begin_contact_queue.emplace_back( pc );
}

void physics_contact_listener::EndContact( b2Contact* contact )
{
	physics_pending_collision pc;

	this->contact = contact;
	manifold = contact->GetManifold();

	pc.entity_a = ( (entity_component*)( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (entity_component*)( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;

	// either entity is dying, don't add this contact to the list.
	if( !pc.entity_a->get_life_cycle()->is_alive() or !pc.entity_b->get_life_cycle()->is_alive() )
	{
		return;
	}

	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	g_engine->end_contact_queue.emplace_back( pc );
}

vec2 physics_contact_listener::calc_hit_normal( b2Body* body_colliding )
{
	vec2 hit_normal = vec2::zero;

	// this handles the case where a circle collides with another circle. this,
	// for some reason, crashes if I try to compute it the same as I compute a
	// collision normal with a box. box2d does something in that case that I
	// don't understand.

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
