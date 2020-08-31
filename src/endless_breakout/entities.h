#pragma once

// ----------------------------------------------------------------------------

struct e_player : w_entity
{
	e_player();
};

// ----------------------------------------------------------------------------

struct e_ball : w_entity
{
	e_ball();

	virtual void update() override;
};
