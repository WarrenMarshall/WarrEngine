#pragma once

struct w_physics_pending_collision
{
	w_entity* entity_a;
	b2Fixture* fixture_a;
	w_entity* entity_b;
	b2Fixture* fixture_b;
	b2Manifold manifold;
	b2WorldManifold wmanifold;
};
