#pragma once

struct w_physics_responder : b2ContactListener
{
	b2Contact* contact = nullptr;
	b2Manifold* manifold = nullptr;
	const char* contact_tags[ 2 ];

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;
	virtual void PreSolve( b2Contact* contact, const b2Manifold* oldManifold ) override;
	virtual void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse ) override;

	[[nodiscard]] bool contact_tags_match( const char* tag_0, const char* tag_1 );
	[[nodiscard]] b2Fixture* find_fixture_from_contact_tag( const char* tag );
	[[nodiscard]] b2Body* find_body_from_contact_tag( const char* tag );
	[[nodiscard]] w_entity_component* find_component_from_contact_tag( const char* tag );
	[[nodiscard]] w_entity* find_entity_from_contact_tag( const char* tag );

	[[nodiscard]] w_vec2 calc_hit_normal( b2Body* body_colliding );

	virtual void update();
};
