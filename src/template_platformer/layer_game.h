#pragma once

struct platformer_layer : w_layer
{
	a_gradient* sky_gradient = nullptr;
	a_sound* music_gameplay = nullptr;

	platformer_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void draw_ui_debug() override;
	virtual w_camera* get_camera() override;

	virtual bool iir_on_pressed( const w_input_event* evt ) override;
	virtual bool iir_on_motion( const w_input_event* evt ) override;
};
