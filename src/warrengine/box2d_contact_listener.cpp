
#include "master_pch.h"
#include "master_header.h"

namespace war::box2d
{

void Contact_Listener::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();

	box2d::Pending_Collision pc;

	pc.entity_a = ( (Entity_Component*)( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (Entity_Component*)( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;

	// if either entity is dying, don't add this contact to the list.
	if( !pc.entity_a->life_cycle.is_alive() or !pc.entity_b->life_cycle.is_alive() )
	{
		return;
	}

	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	g_engine->box2d.begin_contact_queue.push_back( pc );
}

void Contact_Listener::EndContact( b2Contact* contact )
{
	box2d::Pending_Collision pc;

	this->contact = contact;
	manifold = contact->GetManifold();

	pc.entity_a = ( (Entity_Component*)( contact->GetFixtureA()->GetBody()->GetUserData().pointer ) )->parent_entity;
	pc.entity_b = ( (Entity_Component*)( contact->GetFixtureB()->GetBody()->GetUserData().pointer ) )->parent_entity;

	// if either entity is dying, don't add this contact to the list.
	if( !pc.entity_a->life_cycle.is_alive() or !pc.entity_b->life_cycle.is_alive() )
	{
		return;
	}

	pc.fixture_a = contact->GetFixtureA();
	pc.fixture_b = contact->GetFixtureB();
	pc.manifold = *contact->GetManifold();

	g_engine->box2d.end_contact_queue.push_back( pc );
}

Vec2 Contact_Listener::calc_hit_normal( b2Body* body_colliding )
{
	Vec2 hit_normal = Vec2::zero;

	// this handles the case where a circle collides with another circle. this,
	// for some reason, crashes if I try to compute it the same as I compute a
	// collision normal with a box. box2d does something in that case that I
	// don't understand.

	if( manifold->type == 0 )	// circle
	{
		hit_normal = Vec2( body_colliding->GetWorldCenter() ) - Vec2( manifold->localPoint );
	}
	else
	{
		hit_normal = Vec2( manifold->localNormal.x, manifold->localNormal.y );
	}

	return Vec2::normalize( hit_normal );
}

}
