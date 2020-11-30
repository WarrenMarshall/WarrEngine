#include "master_pch.h"
#include "master_header.h"

void w_physics_responder::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();
	contact_tags[ 0 ] = (const char*) contact->GetFixtureA()->GetUserData().pointer;
	contact_tags[ 1 ] = (const char*) contact->GetFixtureB()->GetUserData().pointer;
}

void w_physics_responder::EndContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();
	contact_tags[ 0 ] = (const char*) contact->GetFixtureA()->GetUserData().pointer;
	contact_tags[ 1 ] = (const char*) contact->GetFixtureB()->GetUserData().pointer;
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

bool w_physics_responder::contact_tags_match( const char* tag_0, const char* tag_1 )
{
	return(
		( contact_tags[ 0 ] == tag_0 && contact_tags[ 1 ] == tag_1 )
		|| ( contact_tags[ 0 ] == tag_1 && contact_tags[ 1 ] == tag_0 )
		);
}

// looks at the 2 fixtures involved in this contact and returns the requested one.
b2Fixture* w_physics_responder::find_fixture_from_contact_tag( const char* tag )
{
	if( contact_tags[ 0 ] == tag )
	{
		return contact->GetFixtureA();
	}
	if( contact_tags[ 1 ] == tag )
	{
		return contact->GetFixtureB();
	}

	// contact id not found
	assert( false );

	return nullptr;
}

b2Body* w_physics_responder::find_body_from_contact_tag( const char* tag )
{
	return find_fixture_from_contact_tag( tag )->GetBody();
}

w_entity_component* w_physics_responder::find_component_from_contact_tag( const char* tag )
{
	return (w_entity_component*) ( find_body_from_contact_tag( tag )->GetUserData().pointer );
}

w_entity* w_physics_responder::find_entity_from_contact_tag( const char* tag )
{
	return find_component_from_contact_tag( tag )->parent_entity;
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

void w_physics_responder::update()
{
}
