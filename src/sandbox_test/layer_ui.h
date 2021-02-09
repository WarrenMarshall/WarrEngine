#pragma once

// ----------------------------------------------------------------------------

struct layer_ui_callback : w_imgui_callback
{
	layer_ui_callback();

	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;

	w_imgui_control_data checkbox_data = false;
	w_imgui_control_data slider_01_value = 0.0f;
	w_imgui_control_data slider_02_value = 0.0f;
	w_imgui_control_data edit_text_01_data = std::string( "" );
	w_imgui_control_data edit_text_02_data = std::string( "" );
};

// ----------------------------------------------------------------------------

struct layer_ui : w_layer
{
	layer_ui_callback imgui_callback;

	layer_ui();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
