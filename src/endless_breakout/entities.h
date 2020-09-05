#pragma once

// ----------------------------------------------------------------------------

struct e_paddle : w_entity
{
	e_paddle();
};

// ----------------------------------------------------------------------------

struct e_ball : w_entity
{
	e_ball();

	virtual void collided_with( w_entity* entity_hit, c2Manifold& hit ) final;
};

// ----------------------------------------------------------------------------

struct e_brick : w_entity
{
	e_brick();
};
