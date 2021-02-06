#pragma once

struct w_tween
{
	tweeny::tween<float> tween = {};
	float start = 0.0f, end = 1.0f;

	time_ms time_last;

	w_tween();
	w_tween( float start, float end, time_ms duration_ms, e_tween_type type, e_tween_via via );

	_NODISCARD float operator*();

	void restart();
	void randomize();
	void toggle_direction();
	void set_backwards();
	void set_type( e_tween_type type );
	void set_via( e_tween_via via );
};
