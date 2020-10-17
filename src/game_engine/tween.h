#pragma once

struct w_tween
{
	tweeny::tween<float> tween = {};
	e_tween_type type = tween_type::linear;
	float current_val = 0.0f;

	w_tween( e_tween_type type, float start, float end, int duration_ms );

	void update();
	_NODISCARD float get_fval();
	_NODISCARD int get_ival();
	_NODISCARD int get_ival( int low, int high );
	_NODISCARD bool is_negative();

	void reset_to_start();
	void randomize();
};
