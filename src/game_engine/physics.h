
#pragma once

struct w_contact_listener : b2ContactListener
{
	b2Contact* contact = nullptr;
	std::array< e_sensor_id, 2> sensor_ids;

	// Called when two fixtures begin to touch
	virtual void BeginContact( b2Contact* contact ) override;

	// Called when two fixtures cease to touch
	virtual void EndContact( b2Contact* contact ) override;

	virtual void PreSolve( b2Contact* contact, const b2Manifold* oldManifold ) override;
	virtual void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse ) override;

	bool sensor_ids_match( const std::array<e_sensor_id, 2>& ids_to_check );
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
};
