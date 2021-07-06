#pragma once


namespace war::simple_collision
{

struct pending_collision
{
	entity* entity_a = nullptr;
	entity* entity_b = nullptr;

	vec2 closest_point;
	vec2 normal;
	float depth;

	c2Manifold manifold;
};

}
