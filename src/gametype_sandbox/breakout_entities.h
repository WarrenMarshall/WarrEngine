#pragma once

struct e_breakout_ball : w_entity
{
	w_vec2 dir = w_vec2::zero;
	float speed = 1.25f;

	e_breakout_ball();
	void reset_velocity();
};

// ----------------------------------------------------------------------------

struct e_breakout_paddle : w_entity
{
	e_breakout_paddle();
};

