#pragma once


namespace war::simple_collision
{

struct pending_collision
{
	entity* entity_a;
	entity* entity_b;

	vec2 closest_point;
	vec2 normal;
	float depth;

	c2Manifold manifold;
};

}
