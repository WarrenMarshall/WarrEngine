#pragma once

namespace war::simple_collision
{

struct Pending_Collision final
{
	Entity* entity_a = nullptr;
	Simple_Collision_Body* body_a = nullptr;

	Entity* entity_b = nullptr;
	Simple_Collision_Body* body_b = nullptr;

	Vec2 closest_point = Vec2::zero;
	Vec2 normal = Vec2::zero;
	float_t depth = 0.f;

	c2Manifold manifold = {};
};

}
