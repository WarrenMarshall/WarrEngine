
#pragma once

// ----------------------------------------------------------------------------

struct layer_msg_box_ui_callback : w_imgui_callback
{
	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;

	virtual bool on_input_pressed( const w_input_event* evt ) override;
};

// ----------------------------------------------------------------------------

struct layer_msg_box : w_layer
{
	e_mouse_mode save_mouse_mode;

	layer_msg_box_ui_callback ui_callback;

	virtual void push() override;
	virtual void pop() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
