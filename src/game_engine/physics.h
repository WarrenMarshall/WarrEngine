
#pragma once

struct w_contact_listener : b2ContactListener
{
	b2Contact* contact = nullptr;
	b2Manifold* manifold = nullptr;
	std::array<e_contact_id, 2> contact_ids;

	// Called when two fixtures begin to touch
	virtual void BeginContact( b2Contact* contact ) override;

	// Called when two fixtures cease to touch
	virtual void EndContact( b2Contact* contact ) override;

	virtual void PreSolve( b2Contact* contact, const b2Manifold* oldManifold ) override;
	virtual void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse ) override;

	bool contact_ids_match( const std::array<e_contact_id, 2>& ids_to_check );
	b2Fixture* find_fixture_from_contact_id( e_contact_id id );
	b2Body* find_body_from_contact_id( e_contact_id id );
	w_entity_component* find_component_from_contact_id( e_contact_id id );
	w_entity* find_entity_from_contact_id( e_contact_id id );

	w_vec2 calc_hit_normal( b2Body* body_colliding );
};

// ----------------------------------------------------------------------------

struct w_physics
{
	virtual bool trace_simple( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask );
	virtual bool trace_simple( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_simple* hit_result );
	virtual bool trace_closest( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_closest* hit_result );
	virtual bool trace_all( w_vec2 start, w_vec2 normal, float dist, e_collision_layer layer_mask, w_raycast_all* hit_result );

	virtual bool point_check_simple( w_vec2 pos, e_collision_layer layer_mask );
	virtual bool point_check_simple( w_vec2 pos, e_collision_layer layer_mask, w_query_first* hit_result );
	virtual bool point_check_all( w_vec2 pos, e_collision_layer layer_mask, w_query_all* hit_result );

	virtual void update();
};
