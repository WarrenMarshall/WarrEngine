#pragma once

struct w_tween
{
	tweeny::tween<float> tween = {};
	e_tween_type type = tween_type::linear;
	float current_val = 0.0f;

	w_tween( e_tween_type type, float start, float end, float duration_ms );

	void update();
	float get_fval();
	int get_ival();
	bool is_negative();

	void reset_to_start();
	void randomize();
};
