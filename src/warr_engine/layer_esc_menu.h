
#pragma once

// ----------------------------------------------------------------------------

struct layer_esc_menu_ui_callback : w_imgui_callback
{
	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;
	virtual w_imgui_control_data get_data_for_control( const w_imgui_control& control ) override;

	virtual bool on_input_pressed( const w_input_event* evt ) override;
};

// ----------------------------------------------------------------------------

struct layer_esc_menu : w_layer
{
	e_mouse_mode save_mouse_mode;

	layer_esc_menu_ui_callback ui_callback;
	w_tween window_top_tween;

	virtual void push() override;
	virtual void pop() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
