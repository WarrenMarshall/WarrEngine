#pragma once

struct e_pong_ball : w_entity
{
	w_vec2 dir = w_vec2::zero;
	float speed = 1.25f;

	e_pong_ball();
	void reset_velocity();
};

// ----------------------------------------------------------------------------

struct e_pong_paddle : w_entity
{
	e_pong_paddle();
};

