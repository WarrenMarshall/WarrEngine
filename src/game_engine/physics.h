
#pragma once

struct w_physics_debug_draw : public b2Draw
{
	// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) override;

	// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color ) override;

	// Draw a circle.
	virtual void DrawCircle( const b2Vec2& center, float radius, const b2Color& color ) override;

	// Draw a solid circle.
	virtual void DrawSolidCircle( const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color ) override;

	// Draw a line segment.
	virtual void DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color ) override;

	// Draw a transform. Choose your own length scale.
	// @param xf a transform.
	virtual void DrawTransform( const b2Transform& xf ) override;

	// Draw a point.
	virtual void DrawPoint( const b2Vec2& p, float size, const b2Color& color ) override;
};

// ----------------------------------------------------------------------------

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

	bool contact_ids_match( e_contact_id id_0, e_contact_id id_1 );
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
