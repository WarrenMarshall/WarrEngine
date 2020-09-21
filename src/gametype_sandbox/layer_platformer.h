#pragma once

struct w_contact_listener : b2ContactListener
{
	std::vector<w_vec2> points;
	w_vec2 normal;

	// Called when two fixtures begin to touch
	virtual void BeginContact( b2Contact* contact ) override;

	// Called when two fixtures cease to touch
	virtual void EndContact( b2Contact* contact ) override;

	virtual void PreSolve( b2Contact* contact, const b2Manifold* oldManifold ) override;
	virtual void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse ) override;
};

struct layer_platformer : w_layer
{
	w_contact_listener contact_listener;

	// #todo - replace with use of a sensor
	//w_vec2 player_trace_hit;
	bool player_on_ground = true;

	layer_platformer();

	w_entity* player = nullptr;
	w_entity* player2 = nullptr;
	w_entity* world_geo = nullptr;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt );
};
