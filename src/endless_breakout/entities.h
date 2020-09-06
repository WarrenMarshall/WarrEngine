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

	w_entity_component* ec_fire_trail = nullptr;
};

// ----------------------------------------------------------------------------

struct e_brick : w_entity
{
	e_brick();
};

// ----------------------------------------------------------------------------

struct e_brick_multiball : w_entity
{
	e_brick_multiball();
};
// ----------------------------------------------------------------------------

struct e_brick_fireball : w_entity
{
	e_brick_fireball();
};
