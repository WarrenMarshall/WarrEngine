#pragma once

struct w_contact_listener : b2ContactListener
{
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

	std::unique_ptr<w_timer> timer_jump_limiter = nullptr;
	int player_on_ground = 0;
	int player_drop_down_blocked = 0;

	layer_platformer();

	w_entity* player = nullptr;
	w_entity* player2 = nullptr;
	w_entity* world_geo = nullptr;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt );
};
