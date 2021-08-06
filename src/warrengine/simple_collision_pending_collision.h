#pragma once

namespace war::simple_collision
{

struct pending_collision
{
	entity* entity_a = nullptr;
	ec_simple_collision_body* body_a = nullptr;

	entity* entity_b = nullptr;
	ec_simple_collision_body* body_b = nullptr;

	vec2 closest_point = vec2::zero;
	vec2 normal = vec2::zero;
	float depth = 0.f;

	c2Manifold manifold = {};
};

}
