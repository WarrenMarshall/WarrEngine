#pragma once

struct w_physics_contact_listener : b2ContactListener
{
	b2Contact* contact = nullptr;
	b2Manifold* manifold = nullptr;

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;

	[[nodiscard]] w_vec2 calc_hit_normal( b2Body* body_colliding );
};
