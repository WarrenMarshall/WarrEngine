#pragma once

// ----------------------------------------------------------------------------

struct e_player : w_entity
{
	e_player();

	void draw() override;
};

// ----------------------------------------------------------------------------

struct e_vfx_asteroid_hit : w_entity_cozy
{
	e_vfx_asteroid_hit();
};
