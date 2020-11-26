#pragma once

struct w_physics_responder : b2ContactListener
{
	b2Contact* contact = nullptr;
	b2Manifold* manifold = nullptr;
	const char* contact_ids[ 2 ];

	// Called when two fixtures begin to touch
	virtual void BeginContact( b2Contact* contact ) override;

	// Called when two fixtures cease to touch
	virtual void EndContact( b2Contact* contact ) override;

	virtual void PreSolve( b2Contact* contact, const b2Manifold* oldManifold ) override;
	virtual void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse ) override;

	[[nodiscard]] bool contact_ids_match( const char* id_0, const char* id_1 );
	[[nodiscard]] b2Fixture* find_fixture_from_contact_id( const char* id );
	[[nodiscard]] b2Body* find_body_from_contact_id( const char* id );
	[[nodiscard]] w_entity_component* find_component_from_contact_id( const char* id );
	[[nodiscard]] w_entity* find_entity_from_contact_id( const char* id );

	[[nodiscard]] w_vec2 calc_hit_normal( b2Body* body_colliding );
};
