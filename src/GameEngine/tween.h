#pragma once

struct w_tween
{
	e_tween_type type = tween_type::linear;
	float start = 0.0f, end = 0.0f, step_per_sec = 0.0f;
	float _fval = 0.0f;
	int _ival = 0;
	float _dir = 1.0f;

	w_tween( e_tween_type type, float start, float end, float step_per_sec );
	void refresh_limits();
	void update();
	int get_ival();
	float get_fval();

	void reset_to_start();
	void randomize();
};
