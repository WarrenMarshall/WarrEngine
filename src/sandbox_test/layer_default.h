#pragma once

// ----------------------------------------------------------------------------

struct layer_default_ui_callback : w_imgui_callback
{
	layer_default_ui_callback();

	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;
	virtual void on_motion( const w_imgui_control& control, const w_imgui_result& result ) override;

	w_imgui_control_data checkbox_data = false;
	w_imgui_control_data slider_01_value = 0.0f;
	w_imgui_control_data slider_02_value = 0.0f;
	w_imgui_control_data edit_name_data = std::string( "" );
	w_imgui_control_data edit_email_data = std::string( "" );
};

// ----------------------------------------------------------------------------

struct layer_default : w_layer
{
	a_texture* gradient = nullptr;
	a_texture* tex_hello_world = nullptr;

	layer_default_ui_callback imgui_callback;

	float glow_intensity = 0.0f;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
