#pragma once


namespace war::simple_collision
{

struct pending_collision
{
	entity* entity_a;	// entity that is being touched
	entity* entity_b;	// entity doing the touching

	c2Manifold manifold;
};

}
