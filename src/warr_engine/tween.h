#pragma once

struct w_tween
{
	tweeny::tween<float> tween = {};
	e_tween_type type = tween_type::invalid;
	e_tween_via via = tween_via::invalid;
	float start = 0.0f, end = 1.0f;
	float current_val = 0.0f;

	time_ms time_last;

	w_tween();
	w_tween( float start, float end, time_ms duration_ms, e_tween_type type, e_tween_via via );

	float operator*();

	void restart();
	void randomize();
	void toggle_direction();
	void set_backwards();
	void set_type( e_tween_type type );
	void set_via( e_tween_via via );
};
