#pragma once

// ----------------------------------------------------------------------------

struct e_player : w_entity
{
	e_player();

	virtual void draw();
};

// ----------------------------------------------------------------------------

struct e_asteroid : w_entity
{
	std::unique_ptr<w_timer> timer_flip_fireball = nullptr;

	e_asteroid();

	virtual void update();
};

// ----------------------------------------------------------------------------

struct e_vfx_asteroid_hit : w_entity_cozy
{
	e_vfx_asteroid_hit();
};
