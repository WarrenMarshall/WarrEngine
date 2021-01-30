#pragma once

// ----------------------------------------------------------------------------

struct layer_default_ui_callback : w_imgui_callback
{
	virtual w_imgui_control_data get_data_for_control( const w_imgui_control& control ) override;
	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;
	virtual void on_motion( const w_imgui_control& control, const w_imgui_result& result ) override;

	virtual bool on_input_pressed( const w_input_event* evt ) override;
	virtual bool on_input_held( const w_input_event* evt ) override;
	virtual bool on_input_key( const w_input_event* evt ) override;

	w_imgui_control_data b_checkbox_01 = false;
	w_imgui_control_data slider_value = 0.0f;
	w_imgui_control_data slider_value2 = 0.0f;
	w_imgui_control_data edit_name = std::string( "" );
	w_imgui_control_data edit_email = std::string( "" );
};

// ----------------------------------------------------------------------------

struct layer_default : w_layer
{
	a_texture* gradient = nullptr;
	a_texture* tex_hello_world = nullptr;

	layer_default_ui_callback imgui_callback;

	w_tween glow_tween;
	float glow_intensity = 0.0f;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
