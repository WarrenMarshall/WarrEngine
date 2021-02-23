#pragma once

struct layer_tweens : w_layer
{
	a_texture* tween_tex = nullptr;

	w_tween tween_linear;
	w_tween tween_sin;
	w_tween tween_quad;
	w_tween tween_circular;
	w_tween tween_bounce;
	w_tween tween_back;
	w_tween tween_cubic;
	w_tween tween_elastic;
	w_tween tween_exponential;
	w_tween tween_quartic;
	w_tween tween_quintic;

	w_timer timer_01;
	w_timer timer_02;

	layer_tweens();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
