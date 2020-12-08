
#pragma once

struct layer_esc_menu : w_layer
{
	e_mouse_mode save_mouse_mode;

	virtual void push() override;
	virtual void pop() override;
	virtual void draw_ui() override;

	virtual bool iir_on_pressed( const w_input_event* evt ) override;
};
