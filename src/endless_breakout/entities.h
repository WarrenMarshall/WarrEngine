#pragma once

struct e_background_stars : w_entity
{
	e_background_stars();
};

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
