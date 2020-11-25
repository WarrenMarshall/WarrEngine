
#pragma once

struct w_contact_listener : b2ContactListener
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

// ----------------------------------------------------------------------------

struct w_physics
{
	[[nodiscard]] virtual bool trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask );
	[[nodiscard]] virtual bool trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_simple* hit_result );
	[[nodiscard]] virtual bool trace_closest( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_closest* hit_result );
	[[nodiscard]] virtual bool trace_all( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_all* hit_result );

	[[nodiscard]] virtual bool point_check_simple( w_vec2 pos, bitflags layer_mask );
	[[nodiscard]] virtual bool point_check_simple( w_vec2 pos, bitflags layer_mask, w_query_first* hit_result );
	[[nodiscard]] virtual bool point_check_all( w_vec2 pos, bitflags layer_mask, w_query_all* hit_result );

	virtual void update();

	virtual bool event_input_motion( const w_input_event* evt );
	virtual bool event_input_pressed( const w_input_event* evt );
	virtual bool event_input_held( const w_input_event* evt );
	virtual bool event_input_released( const w_input_event* evt );
};
