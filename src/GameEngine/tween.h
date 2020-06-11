#pragma once

struct w_tween
{
	e_tween_type type = e_tween_type::linear;
	float start = 0.0f, end = 0.0f, step_per_sec = 0.0f;
	float _fval = 0.0f;
	int _ival = 0;
	float _dir = 1.0f;

	/*
		if TRUE, this tween wants to use a fixed step update.
		this is the default. otherwise, it will use update.
	*/
	bool use_fts_update = false;

	w_tween( e_tween_type type, float start, float end, float step_per_sec );
	void update_limits();
	void update();
	void update_fts();
	int get_ival();
	float get_fval();

	void reset_to_start();
	void randomize();
};
