#pragma once

// ----------------------------------------------------------------------------

struct layer_default_ui_callback : w_imgui_callback
{
	virtual e_imgui_control_state get_state_for_control( w_imgui_control* control ) override;
	virtual void was_left_clicked( w_imgui_control* control ) override;

	bool b_checkbox_01 = false;
};

// ----------------------------------------------------------------------------

struct layer_default : w_layer
{
	a_texture* gradient = nullptr;
	a_texture* tex_hello_world = nullptr;

	layer_default_ui_callback imgui_callback;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
